
################################# READ IN DATA #################################

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/0. Raw data/2B")
d1 = read.csv("expt_2B_raw_data.csv"); dim(d1)  # 110

table(duplicated(d1$Subject..))  # they're all unique subjects, as expected

library(car)


################################# CREATE AND RECODE VARIABLES #################################

d2 = d1

# rename columns
names(d2) = c("id", "age", "male", "profession", "recognize", "mean-2", "nice-5", "nice-3", "mean-6",
              "nice-1", "mean-4", "7-blank.1", "nice-2", "mean-5", "mean-3", "nice-6", "mean-1", "nice-4",
              "7-blank.2")

# put in long form
d3 = reshape(d2,
             varying=c("mean-2", "nice-5", "nice-3", "mean-6",
                           "nice-1", "mean-4", "7-blank.1", "nice-2", "mean-5", "mean-3", "nice-6", "mean-1", "nice-4",
                           "7-blank.2"),
             v.names="wager", timevar="face",
             times=c("mean-2", "nice-5", "nice-3", "mean-6",
                     "nice-1", "mean-4", "7-blank.1", "nice-2", "mean-5", "mean-3", "nice-6", "mean-1", "nice-4",
                     "7-blank.2"), direction="long" )


# create eyebrow angle variable
d3$eyebrow = substr( d3$face, 0, 4 )
d3$eyebrow = recode( d3$eyebrow, " 'mean'='b.Mean'; 'nice'='a.Nice'; '7-bl'='c.Blank' ")

# create robot variable
d3$robot = substr( d3$face, 6, 7 )
d3$robot = recode(d3$robot, " '1'='a.Cog'; '2'='b.Kismet'; '3'='c.Big.Lips'; '4'='d.K.bot'; 
                  '5'='e.Fashion.Doll'; '6'='f.Human'; 'nk'='g.Blank' ")
d3$robot = as.factor(d4)

# recode sex
d3$male = d2$male-1

# create child/adult variable
d3$child = d3$age < 18



################################# MAKE EXCLUSIONS #################################

# exclude Castilleja subjects (because they were pilots for a later study)
# and decoy ones
d4 = d3[ (d3$id < 100) & (d3$robot != "g.Blank"), ]

# n=92
table(duplicated(d4$id))


################################# MAKE EXCLUSIONS #################################

# delete extra columms
# remove rows with NA wager (when subject wasn't assigned to that condition)
d5 = d4[ !is.na(d4$wager), -6 ]; dim(d5)

# yes, 6 faces * 92 subjects = 552 rows

# export data
setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/2. Prepped data/2B")
write.csv(d5, paste(Sys.Date(), "prepped_expt_2B_data.csv", sep="_") )


