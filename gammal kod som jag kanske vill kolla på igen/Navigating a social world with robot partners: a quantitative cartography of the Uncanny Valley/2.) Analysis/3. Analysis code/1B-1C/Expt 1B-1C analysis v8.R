
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                                 PRELIMINARIES                                      #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #


setwd("~/Dropbox/a.Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/TIME CAPSULE for OSF/2.) Analysis/2. Prepped data/1B-1C")
l = read.csv("2015-06-17_expt_1B_likeability_long_prepped.csv"); dim(l)  # 5027
t = read.csv("2015-06-17_expt_1C_trust_long_prepped.csv"); dim(t)  # 4943
f = read.csv("2015-06-21_expt_1B_1C_face_means.csv"); dim(f)  # 80


library(car)
library(lme4)
library(lmerTest)
library(Hmisc)
library(plotly)
library(lessR)
library(mediation)
library(ggplot2)


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


# how many people viewed each face on average?
views = aggregate(main.rating ~ face, l, length)$main.rating; mean(views)  # likability
views = aggregate(main.rating ~ face, t, length)$main.rating; mean(views)  # trust


###### ICC of likeability and trust ratings (intra-subject clustering) #####

library(ICC)
ICCest(x=id, y=main.rating, data=l)
ICCest(x=id, y=main.rating, data=t)


# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                                     LIKEABILITY                                    #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

# check whether we should use centered composite to reduce collinearity between poly terms
cor(f$composite, f$composite^2)
cor(f$composite2, f$composite2^2)
# no - collinearity is actually worse in latter case

# look at distribution of face variances (used in inverse-variance weighting)
hist(f$like.sd ^ 2, breaks=30)
summary(1 / f$like.sd ^ 2)
sd(1/f$like.sd ^ 2)


################################## MAIN ANALYSIS MODELS (SOLID LINE) ################################## 

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


################################## ONLY LOW-EMOTION FACES (DASHED LINE) ################################## 

# weighted among only low-emotion faces
f2 = f[f$low.emot==TRUE,]

rs0 = lm( f2$like.mean ~ f2$composite + I(f2$composite^2), weights=1/(f2$like.sd^2) ); summary(rs0)

l.mod.weighted.low.emot = lm( f2$like.mean ~ f2$composite + I(f2$composite^2) +
                       I(f2$composite^3), weights=1/(f2$like.sd^2) ); summary(l.mod.weighted.low.emot)

rs2 = lm( f2$like.mean ~ f2$composite + I(f2$composite^2) +
            I(f2$composite^3) + I(f2$composite^4), weights=1/(f2$like.sd^2) ); summary(rs2)

# compare three models for fit
anova(rs0, l.mod.weighted.low.emot, rs2)
# same


################################## SENSITIVITY: UNWEIGHTED ################################## 

rs0 = lm( f$like.mean ~ f$composite + I(f$composite^2) ); summary(rs0)
l.mod.unweighted = lm( f$like.mean ~ f$composite + I(f$composite^2) +
                         I(f$composite^3) ); summary(l.mod.unweighted)
rs2 = lm( f$like.mean ~ f$composite + I(f$composite^2) +
            I(f$composite^3) + I(f$composite^4) ); summary(rs2)

# compare three models for fit
anova(rs0, l.mod.unweighted, rs2)
# same


################################## DOES EMOTION MODERATE? ################################## 

# does emotion moderate when treated as continuous?  (a priori)
l.emot.main = lm( f$like.mean ~ f$composite + f$mean.emot + I(f$composite^2) +
                    I(f$composite^3), weights=1/(f$like.sd^2) ); summary(l.emot.main)

l.emot.int = lm( f$like.mean ~ f$composite*f$mean.emot + I(f$composite^2)*f$mean.emot +
                   I(f$composite^3)*f$mean.emot, weights=1/(f$like.sd^2) ); summary(l.emot.int)

# does emotion moderate the Uncanny Valley? (test all 3 interaction terms simultaneously)
anova(l.emot.main, l.emot.int)


# does emotion moderate when dichotomized? (in response to reviewer)
l.emot.main = lm( f$like.mean ~ f$composite + f$low.emot + I(f$composite^2) +
                    I(f$composite^3), weights=1/(f$like.sd^2) ); summary(l.emot.main)

l.emot.int = lm( f$like.mean ~ f$composite*f$low.emot + I(f$composite^2)*f$low.emot +
                   I(f$composite^3)*f$low.emot, weights=1/(f$like.sd^2) ); summary(l.emot.int)

# does emotion moderate the Uncanny Valley? (test all 3 interaction terms simultaneously)
anova(l.emot.main, l.emot.int)




################################## SCATTERPLOT WITH FITTED CURVES ################################## 

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
ylab="Likability (-100 to 100)"
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
  theme(text = element_text(size=28) ) +
  
  theme(legend.position="none")  # remove in order to see legend


setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "wild_likeability.svg", sep="_"),
        plot=p1, path=NULL, width=13, height=8, units="in")



##### Simplified Version for Graphical Abstract #####
xlab = ""
ylab="Likability"
half.width = 35
highlight = ifelse(f$face %in% c(76,4,79,66), "red", "black")  # 4 faces to highlight on graph

p3 = ggplot(f, aes(x=composite, y=like.mean, group=highlight) ) +
  geom_rect( aes(xmin=uv.nadir-half.width, xmax=uv.nadir+half.width, ymin=-100, ymax=100),
             fill="#ffeaa8", alpha=0.1 ) +

  #geom_hline(yintercept=0, color="gray", lwd=.6) +  # reference line for neutrality
  geom_point(size=4, color=highlight) +
  #geom_point(aes(shape=emot.valence), color="white", size=9 ) +
  theme_bw() +
  stat_function(fun=eq_w, color="red", lwd=3) +  # weighted regression curve

  xlab(xlab) + ylab(ylab) +
  #scale_y_continuous(breaks=seq(-100, 100, 10))
  scale_x_continuous(breaks=NULL) +
  scale_y_continuous(breaks=NULL) +
  #scale_y_continuous(breaks=seq(-100, 100, 50)) +
  theme(text = element_text(size=28) )

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "graphical_abstract.svg", sep="_"),
        plot=p3, path=NULL, width=10, height=13, units="in")


################################## MAXES AND MINS OF FITTED CURVE ################################## 

# find the initial max
optimize(eq_w, interval=c(-100, 100), maximum=TRUE)

# find the min
optimize(eq_w, interval=c(-100, 100), maximum=FALSE)

# find the global max (fitted value for a fully human face)
optimize(eq_w, interval=c(70, 100), maximum=TRUE)


################################## IN RESPONSE TO REVIEWER ################################## 

# look at distribution of face variances (used in inverse-variance weighting)
hist(f$trust.sd ^ 2, breaks=20)
summary(1 / f$trust.sd ^ 2)
sd(1/f$trust.sd ^ 2)


################################## MAIN ANALYSIS MODELS (SOLID LINE) ################################## 

rs0 = lm( f$trust.mean ~ f$composite + I(f$composite^2),
          weights=1/(f$trust.sd^2) ); summary(rs0)

t.mod.weighted = lm( f$trust.mean ~ f$composite + I(f$composite^2) +
                       I(f$composite^3), weights=1/(f$trust.sd^2) ); summary(t.mod.weighted)

rs2 = lm( f$trust.mean ~ f$composite + I(f$composite^2) +
            I(f$composite^3) + I(f$composite^4), weights=1/(f$trust.sd^2) ); summary(rs2)

# compare three models for fit
anova(rs0, t.mod.weighted, rs2)

# look at residuals
plot(t.mod.weighted)


################################## ONLY LOW-EMOTION FACES (DASHED LINE) ################################## 

t.mod.weighted.low.emot = lm( f2$trust.mean ~ f2$composite, weights=1/(f2$trust.sd^2) )

rs0 = lm( f2$trust.mean ~ f2$composite + I(f2$composite^2), weights=1/(f2$trust.sd^2) )

rs1 = lm( f2$trust.mean ~ f2$composite + I(f2$composite^2) +
                                I(f2$composite^3), weights=1/(f2$trust.sd^2) )

rs2 = lm( f2$trust.mean ~ f2$composite + I(f2$composite^2) +
            I(f2$composite^3) + I(f2$composite^4), weights=1/(f2$trust.sd^2) )

# compare three models for fit
# here, the linear one is good enough
anova(t.mod.weighted.low.emot, rs0, rs1, rs2)



################################## SENSITIVITY: UNWEIGHTED ################################## 

rs0 = lm( f$trust.mean ~ f$composite + I(f$composite^2) ); summary(rs0)

t.mod.unweighted = lm( f$trust.mean ~ f$composite + I(f$composite^2) +
                         I(f$composite^3) ); summary(t.mod.unweighted)

rs2 = lm( f$trust.mean ~ f$composite + I(f$composite^2) +
            I(f$composite^3) + I(f$composite^4) ); summary(rs2)

# compare three models for fit
anova(rs0, t.mod.weighted, rs2)



################################## DOES EMOTION MODERATE? ################################## 

t.emot.main = lm( f$trust.mean ~ f$composite + f$mean.emot + I(f$composite^2) +
                   I(f$composite^3), weights=1/(f$trust.sd^2) ); summary(t.emot.main)

t.emot.int = lm( f$trust.mean ~ f$composite*f$mean.emot + I(f$composite^2)*f$mean.emot +
                I(f$composite^3)*f$mean.emot, weights=1/(f$trust.sd^2) ); summary(t.emot.int)

# does emotion moderate the Uncanny Valley? (test all 3 interaction terms simultaneously)
anova(t.emot.main, t.emot.int)
# nope


# does emotion moderate when dichotomized? (in response to reviewer)
t.emot.main = lm( f$trust.mean ~ f$composite + f$low.emot + I(f$composite^2) +
                    I(f$composite^3), weights=1/(f$trust.sd^2) ); summary(t.emot.main)

t.emot.int = lm( f$trust.mean ~ f$composite*f$low.emot + I(f$composite^2)*f$low.emot +
                   I(f$composite^3)*f$low.emot, weights=1/(f$trust.sd^2) ); summary(t.emot.int)

# does emotion moderate the Uncanny Valley? (test all 3 interaction terms simultaneously)
anova(t.emot.main, t.emot.int)


################################## SCATTERPLOT WITH FITTED CURVES ################################## 

# fitted equations for 3 models
eq_w = function(x) {
  model = t.mod.weighted
  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2) + coef(model)[4]*(x^3)
}

eq_uw = function(x) {
  model = t.mod.unweighted
  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2) + coef(model)[4]*(x^3)
}

eq_w_low_emot = function(x) {
  model = t.mod.weighted.low.emot  # this is the linear model
  coef(model)[1] + coef(model)[2]*x
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
  theme(text = element_text(size=28) ) +
  theme(legend.position="none")  # remove in order to see legend


setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "wild_trust.svg", sep="_"),
        plot=p2, path=NULL, width=13, height=8, units="in")



################################## MAXES AND MINS OF FITTED CURVE ################################## 

# find the initial max
optimize(eq_w, interval=c(-100, 100), maximum=TRUE)

# find the min
optimize(eq_w, interval=c(-100, 100), maximum=FALSE)

# find the global max (fitted value for a fully human face)
optimize(eq_w, interval=c(70, 100), maximum=TRUE)




# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                             EXPLORATORY: CATEGORY CONFUSION                        #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #


################################## MAIN MODELS FOR TIME SPENT ################################## 

rs0 = lm( f$time ~ f$composite, weights=1/(f$time.sd^2) ); summary(rs0)

rs1 = lm( f$time ~ f$composite + I(f$composite^2), weights=1/(f$time.sd^2) ); summary(rs1)

rs2 = lm( f$time ~ f$composite + I(f$composite^2) +
                       I(f$composite^3), weights=1/(f$time.sd^2) ); summary(rs2)

# compare three models for fit
anova(rs0, rs1, rs2)
# second-order model fits best

# look at residuals
plot(rs1)


################################## MAXES AND MINS OF FITTED CURVE ################################## 

eq_w = function(x) {
  model = rs1
  coef(model)[1] + coef(model)[2]*x + coef(model)[3]*(x^2)
}


# find the global max (fitted value for a fully human face)
( time.apex = optimize(eq_w, interval=c(-200, 200), maximum=TRUE)$maximum )


################################## SCATTERPLOT ################################## 

# get CI for the analysis model
fit = predict(rs1, se.fit=TRUE)

predframe = data.frame("time" = fit$fit, "lwr"=fit$fit-1.96*fit$se.fit,
                       "upr"=fit$fit+1.96*fit$se.fit, "composite"=f$composite)

xlab="Mechano-humanness score (-100 to 100)"
ylab="Time (seconds)"
uv.nadir = 35.61862  # as determined above for likeability

p3 = ggplot( data=f, aes(x=composite, y=time) ) + geom_point(size=7) + theme_bw() +
  xlab(xlab) + ylab(ylab) +
  #geom_smooth(color="black", alpha=0.1, lwd=2.2) +
  theme(text = element_text(size=28) ) +
  scale_y_continuous(breaks=seq(10,16,1)) +
  stat_function(fun=eq_w, color="black", lwd=2.2) +  # weighted regression curve
  geom_ribbon(data=predframe, aes(ymin=lwr, ymax=upr), alpha=0.1) + # CI band for main analysis model
 
  geom_vline(x=uv.nadir, lty=2, lwd=1.2) +
  geom_vline(x=time.apex, lty=2, lwd=1.2)
  #annotate("text", x=25, y=10, label="Minimum \nlikability", size=7, angle=0, vjust=-.8) + 
  #annotate("text", x=52, y=10, label="Maximum \ntime", size=7, angle=0, vjust=-.8)
  
setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "wild_time.svg", sep="_"),
        plot=p3, path=NULL, width=13, height=8, units="in")


################################## DOES TIME MEDIATE? ################################## 

library(lavaan)

# make variables for higher-order terms
f$comp.sq = f$composite^2
f$comp.cub = f$composite^3

model <- '
# outcome model 
like.mean ~ a*composite + b*comp.sq + c*comp.cub + d*time

# mediator model
time ~ e*composite + g*comp.sq

# indirect effects (IDE)
timeIDE := d*e + d*g

# total effect
total := a + b + c + d*e + d*g

# proportion mediated
propMed := timeIDE/total
'

fit = sem(model, data=f)
summary(fit, fit.measures=TRUE, standardize=TRUE, rsquare=TRUE)
( boot.fit = parameterEstimates(fit, boot.ci.type="bca.simple") )




# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #
#                      IN RESPONSE TO REVIEWERS                                      #
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ #

################################## IS EFFECT DRIVEN BY JUST THE MOST HUMAN FACES? ##################################

# Reviewer:
# "For the less realistic robots, it could be that people don't have strong opinions whether they like/trust them.
#  Or that humanness doesn't play a role in their likability/trustworthiness ratings and that they only form such
#  opinions for the realistic ones. In the top-20 human-like robots you see a clear increase of likeability as human-likeness
#  increases. This is very close to the UV-story but seems slightly different as it suggests that there is no increase
#  in likeability for low human-likeness."

# look at just the least-human 60 robots (minus top 20)
# see if there is a relationship among these

# refit main analysis model and compare to null model (no effect of human-likeness)
cutoff = sort(f$composite)[60]
temp = f[f$composite <= cutoff,]

null = lm( temp$like.mean ~ 1, weights=1/(temp$like.sd^2) ); summary(null)
rs2 = lm( temp$like.mean ~ temp$composite + I(temp$composite^2) +
            I(temp$composite^3), weights=1/(temp$like.sd^2) ); summary(rs2)

# compare three models for fit
anova(null, rs2)
# yes, there is an effect among low-humanness faces

# trust
null = lm( temp$trust.mean ~ 1, weights=1/(temp$trust.sd^2) ); summary(null)
rs2 = lm( temp$trust.mean ~ temp$composite + I(temp$composite^2) +
            I(temp$composite^3), weights=1/(temp$trust.sd^2) ); summary(rs2)

# compare three models for fit
anova(null, rs2)
# no longer an effect
