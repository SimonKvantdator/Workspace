
################################# READ IN DATA #################################

setwd("~/Dropbox/a.Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/TIME CAPSULE for OSF/2.) Analysis/2. Prepped data/2A")
d = read.csv("2015-06-17_prepped_expt_2A_data.csv")
d = d[,-1]  # remove superfluous column
dim(d)  # 312 X 9

library(lme4)
library(lmerTest)
library(lsmeans)
library(ggplot2)

################################# DESCRIPTIVE STATS #################################

# proportion male and robot owner
prop.table(table(d$male))
prop.table(table(d$own.robot))

# distribution of age is bimodal
ggplot(data=d, aes(x=age)) + geom_histogram()
hist(d$age)
mean(d$age)
median(d$age)

# mean rating by robot
aggregate(vas ~ robot, d, mean)

# 95% CI of ratings for best and worst faces
t.test(d$vas[d$robot=="f.Human"])
t.test(d$vas[d$robot=="d.K.bot"])


################################# MIXED MODELS #################################

# main analysis model
m = lmer( vas ~ robot + (1|id), data=d); summary(m)
red = lmer(vas ~ (1|id), data=d)
anova(m, red)  # compare to reduced model without robot term

# pairwise comparisons of different robots
contrast( lsmeans(m, ~ robot), "pairwise" )


##### Sensitivity Analyses #####

# full model - sensitivity analysis
m = lmer( vas ~ male + age + own.robot + robot + (1|id), data=d); summary(m)
red = lmer(vas ~ male + age + own.robot + (1|id), data=d)
anova(m, red)  # compare to reduced model without robot term


################################# FIGURE 2C #################################

# make dataset with just means and CIs by face
d2 = aggregate( vas ~ robot, data=d, FUN=mean )
d2$ci.lo = aggregate( vas ~ robot, data=d, FUN=function(x) t.test(x)$conf.int[1] )[2]$vas
d2$ci.hi = aggregate( vas ~ robot, data=d, FUN=function(x) t.test(x)$conf.int[2] )[2]$vas

p1 = ggplot(d2, aes(x=robot, y=vas, group=1)) +
  geom_line(lwd=3) + theme_bw() + 
  geom_errorbar( aes(ymin=ci.lo, ymax=ci.hi), width=0.1, lwd=1.3, col="grey" ) +
  geom_point(size=8) +
  #geom_hline(yint=0, lwd=1.5, lty=2) +
  scale_y_continuous(breaks=seq(-30, 40, 10)) +
  theme(text = element_text(size=28) ) +
  ylab("Likeability (-100 to +100)") +
  xlab("Face stimulus") +
  theme(axis.text.x = element_blank()) + 
  annotate("text", x=1, y=d2$ci.hi[1] + 4, label = "a", size=7) +
  annotate("text", x=2, y=d2$ci.hi[2] + 4, label = "b", size=7) +
  annotate("text", x=3, y=d2$ci.hi[3] + 4, label = "a", size=7) +
  annotate("text", x=4, y=d2$ci.hi[4] + 4, label = "a", size=7) +
  annotate("text", x=5, y=d2$ci.hi[5] + 4, label = "b", size=7) +
  annotate("text", x=6, y=d2$ci.lo[6] - 4, label = "c", size=7)
  
setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "composed_likeability.svg", sep="_"),
        plot=p1, path=NULL, width=13, height=8, units="in")



