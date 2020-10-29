
################################# READ IN DATA #################################

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/2. Prepped data/Replication 2A-2B")
l = read.csv("2015-08-28_replic_1A_likeability_long_prepped.csv"); dim(l)  # 630
t = read.csv("2015-08-28_replic_1B_trust_long_prepped.csv"); dim(t)  # 588

# n=105 unique subjects for likability
length(unique(l$id))

# n=98 unique subjects for likability
length(unique(t$id))

library(lme4)
library(lmerTest)
library(lsmeans)
library(ggplot2)

# first obs by subject
first.l = l[!duplicated(l$id),]; dim(first.l)  # n=105
first.t = t[!duplicated(t$id),]; dim(first.t)  # n=98


################################# DESCRIPTIVE #################################

##### Likability #####
# mean likability
mean(l$main.rating, na.rm=TRUE)
t.test(l$main.rating)

# mean by face
aggregate(main.rating ~ face, l, mean)


##### Trust #####
# mean trust
mean(t$main.rating, na.rm=TRUE)
t.test(t$main.rating)

# mean by face
aggregate(main.rating ~ robot, t, mean)


################################# MIXED MODELS #################################

# likability
m1 = lmer(main.rating ~ face + (1|id), data=l); summary(m1)
red = lmer(main.rating ~ (1|id), data=l)
anova(m1, red)
contrast( lsmeans(m1, ~ as.factor(face) ), "pairwise" )

# trust
m2 = lmer(main.rating ~ face + (1|id), data=t); summary(m2)
red = lmer(main.rating ~ (1|id), data=t)
anova(m2, red)
contrast( lsmeans(m2, ~ as.factor(face) ), "pairwise" )



################################# APPENDIX FIGURE: LIKABILITY #################################

# make dataset with just means and CIs by face
l2 = aggregate( main.rating ~ face, data=l, FUN=mean )
l2$ci.lo = aggregate( main.rating ~ face, data=l, FUN=function(x) t.test(x)$conf.int[1] )$main.rating
l2$ci.hi = aggregate( main.rating ~ face, data=l, FUN=function(x) t.test(x)$conf.int[2] )$main.rating


p1 = ggplot( l2, aes(x=face, y=main.rating, group=1) ) +
  geom_errorbar( aes(ymin=ci.lo, ymax=ci.hi), width=0.1, lwd=1.3, col="grey" ) +
  geom_line(lwd=3) + theme_bw() + 
  geom_point(size=8) +       

  scale_y_continuous(breaks=seq(-50, 50, 10)) +
  ylab("Likability (-100 to +100)") +
  xlab("Face stimulus") +

  theme(axis.text.x = element_blank()) +
  theme(text = element_text(size=28) ) +
  
  annotate("text", x=1, y=l2$ci.hi[1] + 3, label = "a", size=7) +
  annotate("text", x=2, y=l2$ci.lo[2] - 3, label = "b", size=7) +
  annotate("text", x=3, y=l2$ci.hi[3] + 3, label = "a, b", size=7) +
  annotate("text", x=4, y=l2$ci.hi[4] + 3, label = "c", size=7) +
  annotate("text", x=5, y=l2$ci.hi[5] + 3, label = "d", size=7) +
  annotate("text", x=6, y=l2$ci.hi[6] + 3, label = "e", size=7)

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "replication_likability.svg", sep="_"),
        plot=p1, path=NULL, width=13, height=8, units="in" )



################################# APPENDIX FIGURE: TRUST #################################

# make dataset with just means and CIs by face
t2 = aggregate( main.rating ~ face, data=t, FUN=mean )
t2$ci.lo = aggregate( main.rating ~ face, data=t, FUN=function(x) t.test(x)$conf.int[1] )$main.rating
t2$ci.hi = aggregate( main.rating ~ face, data=t, FUN=function(x) t.test(x)$conf.int[2] )$main.rating


p2 = ggplot( t2, aes(x=face, y=main.rating, group=1) ) +
  geom_errorbar( aes(ymin=ci.lo, ymax=ci.hi), width=0.1, lwd=1.3, col="grey" ) +
  geom_line(lwd=3) + theme_bw() + 
  geom_point(size=8) +       
  
  scale_y_continuous(breaks=seq(0, 70, 5)) +
  ylab("Trust ($ wagered, 0 to 100)") +
  xlab("Face stimulus") +
  
  theme(axis.text.x = element_blank()) +
  theme(text = element_text(size=28) ) +
  
  annotate("text", x=1, y=t2$ci.lo[1] - 2, label = "a", size=7) +
  annotate("text", x=2, y=t2$ci.lo[2] - 2, label = "a", size=7) +
  annotate("text", x=3, y=t2$ci.hi[3] + 2, label = "a", size=7) +
  annotate("text", x=4, y=t2$ci.hi[4] + 2, label = "a", size=7) +
  annotate("text", x=5, y=t2$ci.hi[5] + 2, label = "a", size=7) +
  annotate("text", x=6, y=t2$ci.hi[6] + 2, label = "a", size=7)

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "replication_trust.svg", sep="_"),
        plot=p2, path=NULL, width=13, height=8, units="in")



