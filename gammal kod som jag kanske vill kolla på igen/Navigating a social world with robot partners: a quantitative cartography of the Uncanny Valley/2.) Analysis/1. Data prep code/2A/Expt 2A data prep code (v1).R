
################################# READ IN DATA #################################

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/0. Raw data/2A")
d1 = read.csv("expt_2A_raw_data.csv"); dim(d1)  # 52

library(car)


################################# CREATE AND RECODE VARIABLES #################################

d2 = d1[, -3]

# rename columns
names(d2) = c("id", "sex", "age", "video.game.hrs.wk", "own.robot", "a.Cog", "e.Fashion.Doll", "Decoy.1", "b.Kismet",
              "f.Human", "Decoy.2", "d.K.bot", "Decoy.3", "c.Big.Lips")


# put in long form
d3 = reshape(d2,
             varying=c("a.Cog", "e.Fashion.Doll", "Decoy.1", "b.Kismet",
                       "f.Human", "Decoy.2", "d.K.bot", "Decoy.3", "c.Big.Lips"),
             v.names="vas", timevar="robot",
             times=c("a.Cog", "e.Fashion.Doll", "Decoy.1", "b.Kismet",
                     "f.Human", "Decoy.2", "d.K.bot", "Decoy.3", "c.Big.Lips"), direction="long" )

# recode sex
d3$male[d3$sex=="M"] = 1
d3$male[d3$sex=="F"] = 0

# create child/adult variable
d3$child = d3$age < 18


################################# EXCLUDE OBSERVATIONS #################################

# remove decoy face observations
d4 = d3[ ! d3$robot %in% c("Decoy.1", "Decoy.2", "Decoy.3"), ]

# delete extra columms
d5 = d4[ , -2 ]


################################# EXPORT DATA #################################

# export data
setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/2. Prepped data/2A")
write.csv(d4, paste(Sys.Date(), "prepped_expt_2A_data.csv", sep="_") )


