
################################# READ IN DATA #################################


setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/2.) Analysis/2. Prepped data/2B")
d = read.csv("2015-06-17_prepped_expt_2B_data.csv")
d = d[,-1]
dim(d)  # 552

# n=92 unique subjects :)
length(unique(d$id))

library(lme4)
library(lmerTest)
library(lsmeans)
library(ggplot2)

# look at first row per subject for demographics
first.dat = d[!duplicated(d$id),]
median(first.dat$age, na.rm=TRUE)


################################# EXPLORE #################################

# how many ratings did each face get?
table(d$eyebrow, d$robot)

# older people tend to bet more
ggplot(d, aes(x=age, y=wager)) + geom_point(position="jitter") + theme_bw()
cor.test(d$age, d$wager)


################################# MEAN WAGER BY FACE #################################

nice = d[d$eyebrow=="a.Nice",]

# mean wager
mean(nice$wager, na.rm=TRUE)
t.test(nice$wager)

# mean wager by face
aggregate(wager ~ robot, d, mean)

# 95% CI for mean wager for best and worst faces
t.test(nice$wager[nice$robot=="b.Kismet"])
t.test(nice$wager[nice$robot=="d.K.bot"])


################################# MIXED MODELS #################################

# main model IN PAPER: UV effect
m1 = lmer(wager ~ robot + (1|id), data=nice); summary(m1)
red = lmer(wager ~ (1|id), data=nice); summary(red)
anova(m1,red)
contrast( lsmeans(m1, ~ as.factor(robot) ), "pairwise" )

# main model IN PAPER: eyebrow effect
m1 = lmer(wager ~ robot + (1|id), data=d); summary(m1)
m2 = lmer(wager ~ robot + eyebrow + (1|id), data=d); summary(m2)
confint(m2)
anova(m1, m2)


# exploratory: interaction of eyebrow with robot
m1 = lmer(wager ~ robot + eyebrow + (1|id), data=d); summary(m1)
m2 = lmer(wager ~ robot * eyebrow + (1|id), data=d); summary(m2)
anova(m1, m2)
# not a moderator


################################# FIGURE 2C #################################

# make dataset with just means and CIs by face
d2 = aggregate( wager ~ robot + eyebrow, data=d, FUN=mean )
d2$ci.lo = aggregate( wager ~ robot + eyebrow, data=d, FUN=function(x) t.test(x)$conf.int[1] )$wager
d2$ci.hi = aggregate( wager ~ robot + eyebrow, data=d, FUN=function(x) t.test(x)$conf.int[2] )$wager


##### Figure: neutral only #####
temp = d2[d2$eyebrow=="a.Nice",]

p1 = ggplot( temp, aes(x=robot, y=wager, group=1) ) +
  geom_errorbar( aes(ymin=ci.lo, ymax=ci.hi), width=0.1, lwd=1.3, col="grey" ) +
  geom_line(lwd=3) + theme_bw() + 
  geom_point(size=8) +       

  scale_y_continuous(breaks=seq(0, 70, 5)) +
  ylab("Trust ($ wagered, 0 to 100)") +
  xlab("Face stimulus") +

  theme(axis.text.x = element_blank()) +
  theme(text = element_text(size=28) ) +
  
  annotate("text", x=1, y=d2$ci.hi[1] + 3, label = "a, b", size=7) +
  annotate("text", x=2, y=d2$ci.lo[2] - 3, label = "b", size=7) +
  annotate("text", x=3, y=d2$ci.hi[3] + 3, label = "a, b", size=7) +
  annotate("text", x=4, y=d2$ci.hi[4] + 3, label = "a", size=7) +
  annotate("text", x=5, y=d2$ci.hi[5] + 3, label = "a", size=7) +
  annotate("text", x=6, y=d2$ci.hi[6] + 3, label = "a", size=7)

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "composed_neutral_only_trust.svg", sep="_"),
        plot=p1, path=NULL, width=13, height=8, units="in")



##### Appendix Figure: angled vs. neutral #####
# remove CIs when eyebrow is nice to unclutter plot
d2$ci.lo[d2$eyebrow=="a.Nice"] = NA
d2$ci.hi[d2$eyebrow=="a.Nice"] = NA

p2 = ggplot(d2, aes(x=robot, y=wager, group=eyebrow)) +
  
  geom_errorbar( aes(ymin=ci.lo, ymax=ci.hi), width=0.1, lwd=1.3, col="grey" ) +
  geom_line( aes(size=eyebrow, lty=eyebrow) ) +
  geom_point( aes(shape=eyebrow), size=8 ) +
  
  scale_size_discrete(range=c(.8,3), labels=c("Neutral eyebrows", "Angled eyebrows"), name="", guide=guide_legend("bottom") ) +
  scale_linetype_manual(values=c(2,1), labels=c("Neutral eyebrows", "Angled eyebrows"), name="") +
  scale_shape_manual(values=c(32, 19), guide="none" ) +
  
  scale_y_continuous(breaks=seq(0, 70, 5)) +
  ylab("Trust ($ wagered, 0 to 100)") +
  xlab("Face stimulus") +
  theme_bw() +
  theme(axis.text.x = element_blank()) +
  theme(text = element_text(size=28) ) +
  
  theme(legend.position="none")  # remove in order to see legend
  

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "composed_eyebrow_comparison_trust.svg", sep="_"),
        plot=p2, path=NULL, width=13, height=8, units="in")

