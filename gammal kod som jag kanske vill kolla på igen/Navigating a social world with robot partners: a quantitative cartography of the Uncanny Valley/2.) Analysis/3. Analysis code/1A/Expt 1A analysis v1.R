
####################### READ IN DATA #######################

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/2. Prepped data/1A")

# long data: one row per subject-face combination
l = read.csv("2015-06-17_exp_1A_long_data_prepped.csv"); dim(l)  # 5280

# face means data
f = read.csv("2015-06-17_exp_1A_face_means_prepped.csv"); dim(f)  # 80 (1 row per face)
  

library(plotly)
library(lessR)



####################### DEMOGRAPHICS #######################

# time spent per face (over all subjects and all faces)
summary(l$time.spent)

# use only first row per subject
first = l[!duplicated(l$survey_code), ]
summary(first)

SummaryStats(first, n.cat=3)

# were robot ratings different by sex?
summary( lmer(main.rating ~ female + rating.variable + (1|survey_code), data=l) )
# no



####################### FIGURES #######################

##### Scatterplot in Appendix #####

# scatterplot of mean ratings by face (numbered)
xlab="Human-resemblance (0 to 100)"
ylab="Mechanical-resemblance (0 to 100)"

p1 = ggplot( data=f, aes( x=mean.human, y=mean.mech, label=face ) ) + geom_point(size=4) +
  theme_bw() + xlab(xlab) + ylab(ylab) +
  theme(text = element_text(size=28) ) +
  scale_x_continuous(breaks=seq(0,100,10)) +
  scale_y_continuous(breaks=seq(0,100,10))

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/Manuscript/Figures")
ggsave( filename = paste(Sys.Date(), "wild_human_mechanical_scatter.svg", sep="_"),
        plot=p1, path=NULL, width=13, height=13, units="in")


##### Not in Paper: Explore Graphically #####

# boxplot of ratings by face, stratified by rating variable (human-like/mechanical)
ggplot( data=l, aes( x=face, y=main.rating, group=survey_code ) ) + geom_boxplot() +
  facet_grid(rating.variable ~ .)

# scatterplot of mean ratings by face (numbered)
ggplot( data=f, aes( x=mean.human, y=mean.mech, label=face ) ) + geom_text() +
  theme_bw()

# plotly version of the scatterplot (points)
p = ggplot( data=f, aes( x=mean.human, y=mean.mech ) ) + geom_point() +
  theme_bw()
py = plotly()
r = py$ggplotly(p)


col = c("red", "black")
labels = c("'High'-emotion (magnitude > 10)", "Low-emotion (magnitude < 10)")
ggplot( data=f, aes( x=mean.human, y=mean.mech, label=face, color=as.factor(low.emot)) ) + geom_text() +
  theme_bw() + scale_color_manual(values=col, labels=labels, name="")


# density plot: mean human-likeness
ggplot( data=f, aes(x=mean.human) ) + geom_histogram( aes(y=..density..) ) +
  geom_density(adjust=0.33, alpha=.2, fill="#FF6666") + theme_bw()

# density plot: mean mechanical-ness
ggplot( data=f, aes(x=mean.mech) ) + geom_histogram( aes(y=..density..) ) +
  geom_density(adjust=0.33, alpha=.2, fill="#FF6666") + theme_bw()

# emotional valence
# with lines corresponding to faces with "low emotion" (emotion magnitude < 10)
ggplot( data=f, aes(x=mean.emot) ) + geom_histogram( aes(y=..density..) ) +
  geom_density(adjust=0.33, alpha=.2, fill="#FF6666") + theme_bw() +
  geom_vline(aes(xintercept=-10),
             color="red", linetype="dashed", size=1) +
  geom_vline(aes(xintercept=10),
             color="red", linetype="dashed", size=1)

# emotion vs. human mean
ggplot( data=f, aes(x=mean.human, y=mean.emot) ) + geom_point() +
  theme_bw()

# emotion vs. human mean
ggplot( data=f, aes(x=mean.mech, y=mean.emot) ) + geom_point() +
  theme_bw()



####################### MAIN ANALYSES #######################

# correlation matrix of mean human, mech, and emotion
cor(f[,3:5])

# is mean human-likeness highly correlated with mean mechanical-ness?
# across faces
cor.test(f$mean.human, f$mean.mech)


# mean emotion is NOT correlated with mean human/mean mechanical
cor.test(f$mean.emot, f$mean.human)
cor.test(f$mean.emot, f$mean.mech)


# descriptive stuff for human scales
summary(f$mean.human); sd(f$mean.human)
summary(f$mean.mech); sd(f$mean.mech)
summary(f$mean.emot)



