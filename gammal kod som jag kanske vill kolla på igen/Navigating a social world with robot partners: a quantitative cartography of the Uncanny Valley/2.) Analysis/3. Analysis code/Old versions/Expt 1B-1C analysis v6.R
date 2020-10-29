
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                                 PRELIMINARIES                                      #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/2. Prepped data/1B-1C")

l = read.csv("2015-06-17_expt_1B_likeability_long_prepped.csv"); dim(l)  # 5027
t = read.csv("2015-06-17_expt_1C_trust_long_prepped.csv"); dim(t)  # 4943
f = read.csv("2015-06-17_expt_1B_1C_face_means.csv"); dim(f)  # 80


library(car)
library(lme4)
library(lmerTest)
library(Hmisc)
library(plotly)
library(lessR)
library(mediation)


###### Explore #####

# humanness by L and T with LOESS
scatterplot(f$composite, f$like.mean)
scatterplot(f$composite, f$trust.mean)

# scatterplot of human-like vs. mechanical (numbered)
ggplot( data=f, aes( x=mean.human, y=mean.mech, label=face ) ) + geom_text() +
  theme_bw()

# scatterplot of human-like vs. mechanical (not numbered)
xlab="Mechanical-resemblence (0 to 100)"
ylab="Human-resemblence (0 to 100)"
ggplot( data=f, aes( x=mean.human, y=mean.mech ) ) + geom_point(size=4) +
  theme_bw() +
  xlab(xlab) + ylab(ylab) +
  theme(text = element_text(size=20) )


###### Demographics #####

# take only first obs for likeability subject group
first.l = l[!duplicated(l$id),]; dim(first.l)
SummaryStats(first.l, n.cat=3)

# take only first obs for trust subject group
first.t = t[!duplicated(t$id),]; dim(first.t)
SummaryStats(first.t, n.cat=3)

# time spent per face
summary(l$time.spent)
summary(t$time.spent)

# were Exp 1B-C subjects higher proportion male than Exp 1A?
# 1A: 37/66
# 1B-1C: (135+141) / (342 + 334)
prop.test(x=c(37, 135+141), n = c(66, 342 + 334))
# yes, significantly more females in the first sample

# were robot face ratings different by sex?


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                                     LIKEABILITY                                    #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

# check whether we should use centered composite to reduce collinearity between poly terms
cor(f$composite, f$composite^2)
cor(f$composite2, f$composite2^2)
# no - collinearity is actually worse in latter case

##### Main Analysis #####
# polynomial regression, inverse-weighted by likeability variance

# http://ww2.coastal.edu/kingw/statistics/R-tutorials/simplenonlinear.html
# poly regression on means data
rs0 = lm( f$like.mean ~ f$composite + I(f$composite^2), weights=1/(f$like.sd^2) ); summary(rs0)

l.mod.weighted = lm( f$like.mean ~ f$composite + I(f$composite^2) +
                       I(f$composite^3), weights=1/(f$like.sd^2) ); summary(l.mod.weighted)

rs2 = lm( f$like.mean ~ f$composite + I(f$composite^2) +
            I(f$composite^3) + I(f$composite^4), weights=1/(f$like.sd^2) ); summary(rs2)

# compare three models for fit
anova(rs0, l.mod.weighted, rs2)
# third-order model is best

# look at residuals
plot(l.mod.weighted)


##### Sensitivity Analysis: Weighted Among Low-Emotion Faces #####
f2 = f[f$low.emot==TRUE,]
rs0 = lm( f2$like.mean ~ f2$composite + I(f2$composite^2), weights=1/(f2$like.sd^2) ); summary(rs0)
l.mod.weighted.low.emot = lm( f2$like.mean ~ f2$composite + I(f2$composite^2) +
                       I(f2$composite^3), weights=1/(f2$like.sd^2) ); summary(l.mod.weighted.low.emot)
rs2 = lm( f2$like.mean ~ f2$composite + I(f2$composite^2) +
            I(f2$composite^3) + I(f2$composite^4), weights=1/(f2$like.sd^2) ); summary(rs2)

# compare three models for fit
anova(rs0, l.mod.weighted.low.emot, rs2)
# same


##### Sensitivity Analysis: Unweighted #####
rs0 = lm( f$like.mean ~ f$composite + I(f$composite^2) ); summary(rs0)
l.mod.unweighted = lm( f$like.mean ~ f$composite + I(f$composite^2) +
                         I(f$composite^3) ); summary(l.mod.unweighted)
rs2 = lm( f$like.mean ~ f$composite + I(f$composite^2) +
            I(f$composite^3) + I(f$composite^4) ); summary(rs2)

# compare three models for fit
anova(rs0, l.mod.unweighted, rs2)
# same



##### Scatterplot with Fitted Models #####

# fitted equations for 3 models
eq_w = function(x) {
  model = l.mod.weighted
  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2) + coef(model)[4]*(x^3)
}

eq_uw = function(x) {
  model = l.mod.unweighted
  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2) + coef(model)[4]*(x^3)
}

eq_w_low_emot = function(x) {
  model = l.mod.weighted.low.emot
  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2) + coef(model)[4]*(x^3)
}



# get CI for the analysis model
fit = predict(l.mod.weighted, se.fit=TRUE)
predframe = data.frame("like.mean" = fit$fit, "lwr"=fit$fit-1.96*fit$se.fit,
                       "upr"=fit$fit+1.96*fit$se.fit, "composite"=f$composite)

xlab="Mechano-humanness score (-100 to 100)"
ylab="Likeability (-100 to 100)"
shapes = c(45, 43)

p1 = ggplot(f, aes(x=composite, y=like.mean) ) +
  #geom_hline(yintercept=0, color="gray", lwd=.6) +  # reference line for neutrality
  geom_point(size=7) +
  geom_point(aes(shape=emot.valence), color="white", size=9 ) +
  theme_bw() +
  stat_function(fun=eq_w, color="black", lwd=2.2) +  # weighted regression curve
  #stat_function(fun=eq_uw, color="blue") +  # unweighted regression curve
  #geom_smooth() +  # LOESS
  stat_function(fun=eq_w_low_emot, color="black", lty=2, lwd=1) + # weighted regression curve, low emotion
  geom_ribbon(data=predframe, aes(ymin=lwr, ymax=upr), alpha=0.1) +  # CI band for main analysis model
  xlab(xlab) + ylab(ylab) +
  scale_x_continuous(breaks=seq(-100, 100, 25)) +
  scale_y_continuous(breaks=seq(-100, 100, 25)) +
  #scale_color_manual(values=col, labels=labels, name="") +
  scale_shape_manual(values=shapes) +
  theme(text = element_text(size=20) ) +
  
  theme(legend.position="none")  # remove in order to see legend


setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "fig_5_likeability.svg", sep="_"),
        plot=p1, path=NULL, width=13, height=8, units="in")



##### Maxes and Mins of the Fitted Curve ##### 

# find the initial max
optimize(eq_w, interval=c(-100, 100), maximum=TRUE)

# find the min
optimize(eq_w, interval=c(-100, 100), maximum=FALSE)

# find the global max (fitted value for a fully human face)
optimize(eq_w, interval=c(70, 100), maximum=TRUE)







# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                                     TRUST                                          #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

##### Main Analysis #####
# polynomial regression, inverse-weighted by likeability variance

# http://ww2.coastal.edu/kingw/statistics/R-tutorials/simplenonlinear.html
# poly regression on means data
rs0 = lm( f$trust.mean ~ f$composite + I(f$composite^2), weights=1/(f$trust.sd^2) ); summary(rs0)

t.mod.weighted = lm( f$trust.mean ~ f$composite + I(f$composite^2) +
                       I(f$composite^3), weights=1/(f$trust.sd^2) ); summary(t.mod.weighted)

rs2 = lm( f$trust.mean ~ f$composite + I(f$composite^2) +
            I(f$composite^3) + I(f$composite^4), weights=1/(f$trust.sd^2) ); summary(rs2)

# compare three models for fit
anova(rs0, t.mod.weighted, rs2)

# look at residuals
plot(t.mod.weighted)


##### Sensitivity Analysis: Unweighted #####

rs0 = lm( f$trust.mean ~ f$composite + I(f$composite^2) ); summary(rs0)
t.mod.weighted = lm( f$trust.mean ~ f$composite + I(f$composite^2) +
                       I(f$composite^3) ); summary(t.mod.weighted)
rs2 = lm( f$trust.mean ~ f$composite + I(f$composite^2) +
            I(f$composite^3) + I(f$composite^4) ); summary(rs2)

# compare three models for fit
anova(rs0, t.mod.weighted, rs2)


##### Sensitivity Analysis: Weighted Among Low-Emotion Faces #####
rs0 = lm( f2$trust.mean ~ f2$composite + I(f2$composite^2), weights=1/(f2$trust.sd^2) ); summary(rs0)
t.mod.weighted.low.emot = lm( f2$trust.mean ~ f2$composite + I(f2$composite^2) +
                                I(f2$composite^3), weights=1/(f2$trust.sd^2) ); summary(t.mod.weighted.low.emot)
rs2 = lm( f2$trust.mean ~ f2$composite + I(f2$composite^2) +
            I(f2$composite^3) + I(f2$composite^4), weights=1/(f2$trust.sd^2) ); summary(rs2)

# compare three models for fit
# here, the second-order is good enough
anova(rs0, t.mod.weighted.low.emot, rs2)


##### Sensitivity Analysis: With Emotion Interactions #####
t.emot.int = lm( f$trust.mean ~ f$composite*f$mean.emot + I(f$composite^2)*f$mean.emot +
                I(f$composite^3)*f$mean.emot, weights=1/(f$trust.sd^2) ); summary(t.emot.int)

# does emotion moderate the Uncanny Valley? (test all 3 interaction terms simultaneously)
anova(t.mod.weighted, t.emot.int)



##### Scatterplot with 2 Fitted Models + LOESS #####

# fitted equations for 3 models
eq_w = function(x) {
  model = t.mod.weighted
  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2) + coef(model)[4]*(x^3)
}

eq_uw = function(x) {
  model = t.mod.unweighted
  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2) + coef(model)[4]*(x^3)
}

#eq_w_low_emot = function(x) {
#  model = t.mod.weighted.low.emot
#  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2) + coef(model)[4]*(x^3)
#}


eq_w_low_emot = function(x) {
  model = rs0  # this is the 2nd-order model
  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2)
}


# get CI for the analysis model
fit = predict(t.mod.weighted, se.fit=TRUE)

predframe = data.frame("trust.mean" = fit$fit, "lwr"=fit$fit-1.96*fit$se.fit,
                       "upr"=fit$fit+1.96*fit$se.fit, "composite"=f$composite)

xlab="Mechano-humanness score (-100 to 100)"
ylab="Trust ($ wagered, 0 to 100)"
col = c("red", "black")
shapes = c(45, 43)


p2 = ggplot(f, aes(x=composite, y=trust.mean) ) +
  #geom_hline(yintercept=0, color="gray", lwd=.6) +  # reference line for neutrality
  geom_point(size=7) +
  geom_point(aes(shape=emot.valence), color="white", size=9 ) +
  theme_bw() +
  stat_function(fun=eq_w, color="black", lwd=2.2) +  # weighted regression curve
  #stat_function(fun=eq_uw, color="blue") +  # unweighted regression curve
  #geom_smooth() +  # LOESS
  stat_function(fun=eq_w_low_emot, color="black", lty=2, lwd=1) + # weighted regression curve, low emotion
  geom_ribbon(data=predframe, aes(ymin=lwr, ymax=upr), alpha=0.1) +  # CI band for main analysis model
  xlab(xlab) + ylab(ylab) +
  scale_x_continuous(breaks=seq(-100, 100, 25)) +
  scale_y_continuous(breaks=seq(0, 50, 5)) +
  #scale_color_manual(values=col, labels=labels, name="") +
  scale_shape_manual(values=shapes) +
  theme(text = element_text(size=20) ) +
  theme(legend.position="none")  # remove in order to see legend


setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "fig_5_trust.svg", sep="_"),
        plot=p2, path=NULL, width=13, height=8, units="in")



##### Maxes and Mins of the Fitted Curve ##### 

# find the initial max
optimize(eq_w, interval=c(-100, 100), maximum=TRUE)

# find the min
optimize(eq_w, interval=c(-100, 100), maximum=FALSE)

# find the global max (fitted value for a fully human face)
optimize(eq_w, interval=c(70, 100), maximum=TRUE)




# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                               TIME SPENT AS MEDIATOR                               #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #


##### Fit Regression for time2 Spent ####
rs0 = lm( f$time ~ f$composite, weights=1/(f$time.sd^2) ); summary(rs0)

rs1 = lm( f$time ~ f$composite + I(f$composite^2), weights=1/(f$time.sd^2) ); summary(rs1)

rs2 = lm( f$time ~ f$composite + I(f$composite^2) +
                       I(f$composite^3), weights=1/(f$time.sd^2) ); summary(rs2)

rs3 = lm( f$time ~ f$composite + I(f$composite^2) +
            I(f$composite^3) + I(f$composite^4), weights=1/(f$time.sd^2) ); summary(rs3)


# compare three models for fit
anova(rs0, rs1, rs2, rs3)
# second-order model fits best

# look at residuals
plot(rs1)


##### Maxes and Mins of the Fitted Curve ##### 

eq_w = function(x) {
  model = rs1
  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2)
}


# find the global max (fitted value for a fully human face)
( time.apex = optimize(eq_w, interval=c(-200, 200), maximum=TRUE)$maximum )


##### "Unfolded" Plot #####

# get CI for the analysis model
fit = predict(rs1, se.fit=TRUE)

predframe = data.frame("time" = fit$fit, "lwr"=fit$fit-1.96*fit$se.fit,
                       "upr"=fit$fit+1.96*fit$se.fit, "composite"=f$composite)

xlab="Mechano-humanness score (-100 to 100)"
ylab="Time (seconds)"
uv.nadir = 36.4841

p3 = ggplot( data=f, aes(x=composite, y=time) ) + geom_point(size=7) + theme_bw() +
  xlab(xlab) + ylab(ylab) +
  #geom_smooth(color="black", alpha=0.1, lwd=2.2) +
  theme(text = element_text(size=20) ) +
  scale_y_continuous(breaks=seq(10,16,1)) +
  stat_function(fun=eq_w, color="black", lwd=2.2) +  # weighted regression curve
  geom_ribbon(data=predframe, aes(ymin=lwr, ymax=upr), alpha=0.1) + # CI band for main analysis model
 
  geom_vline(x=uv.nadir, lty=2, lwd=1.2) +
  geom_vline(x=time.apex, lty=2, lwd=1.2)
  #annotate("text", x=25, y=10, label="Minimum \nlikability", size=7, angle=0, vjust=-.8) + 
  #annotate("text", x=52, y=10, label="Maximum \ntime", size=7, angle=0, vjust=-.8)
  
setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "fig_5_time.svg", sep="_"),
        plot=p3, path=NULL, width=13, height=8, units="in")




library(MBESS)
mediation(x = f$composite, mediator = f$time, dv = f$like.mean)
mediation(x = f$composite^2, mediator = f$time, dv = f$like.mean)
mediation(x = f$composite^3, mediator = f$time, dv = f$like.mean)





##### Composite -> Time Spent (Confusion) -> Likeability #####


# Regression 2: X -> M

scatterplot(f$composite, f$time)
X.M = lm( time ~ composite + I(composite^2), data=f ); summary(X.M)

# Regression 3: M -> Y
scatterplot(f$time, f$like.mean)
M.Y = lm( like.mean ~ time, data=f ); summary(M.Y)




##### Judd/Kenny Approach #####

unmed = lm( like.mean ~ composite + I(composite^2) + I(composite^3), data=f ); summary(unmed)
  
med = lm( like.mean ~ time + composite + I(composite^2) + I(composite^3), data=f ); summary(med)






outcome.fit = lm( like.mean ~ time + composite + I(composite^2) +
                I(composite^3), data=f ); summary(outcome.fit)

med.out = mediate(med.fit, outcome.fit, treat="composite", mediator="time")
  
  




# "folded" version
# hypothesis: it will take longer to rate faces (on mechanical- or human-resemblance) 
#  that are near the UV

ggplot( data=f, aes(x=nadir.dist, y=time) ) + geom_point(size=7) + theme_bw() +
  xlab("Absolute distance from UV nadir") + ylab("Time spent rating mech/human resemblance") +
  geom_smooth(color="black")

m1 = lm( time ~ nadir.dist, data=f ); summary(m1)

m2 = lm( time ~ nadir.dist + I(nadir.dist^2), data=f ); summary(m2)

anova(m1, m2)
# p = 0.09
  

##### Sensitivity Analysis: Exclude 1 Influential Outlier #####

# it is the one with the highest median time
temp = f[f$time < max(f$time),]

ggplot( data=temp, aes(x=nadir.dist, y=time) ) + geom_point(size=3) + theme_bw() +
  xlab("Absolute distance from UV nadir") + ylab("Time spent rating mech/human resemblance") +
  geom_smooth()

m1 = lm( time ~ nadir.dist, data=temp ); summary(m1)

m2 = lm( time ~ nadir.dist + I(nadir.dist^2), data=temp ); summary(m2)

anova(m1, m2)
# p = 0.02



