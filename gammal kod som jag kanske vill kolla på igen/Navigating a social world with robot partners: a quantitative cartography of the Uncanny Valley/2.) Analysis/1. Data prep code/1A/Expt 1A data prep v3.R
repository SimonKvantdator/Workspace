
####################### READ IN DATA ####################### 

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/0. Raw data/1A")
d1 = read.csv("1A_raw_qualtrics_minimally_cleaned_no_header.csv"); dim(d1)  # 179

library(reshape2)
library(ggplot2)
library(data.table)
library(car)

####################### RENAME VARIABLES ####################### 

d2 = d1

# human or mechanical rating variable
var.index = grep( "Q24_", names(d2) )
face.names = paste("face.", 1:81, sep="")
names(d2)[ var.index ] = face.names

# emotion
var.index = grep( "Q20_1.", names(d2) )
emot.names = paste("emot.face.", 1:81, sep="")
names(d2)[ var.index ] = emot.names

# have you seen face before?
var.index = grep( "Q20.", names(d2) )
seen.names = paste("seen.face.", 1:81, sep="")
names(d2)[ var.index ] = seen.names

# time spent on each face
var.index = grep( "Q4_3.", names(d2) )
time.names = paste("time.face.", 1:81, sep="")
names(d2)[ var.index ] = time.names

names(d2)[ names(d2) == "Q18" ] = "problems"
names(d2)[ names(d2) == "Q16" ] = "sex"
names(d2)[ names(d2) == "Q15" ] = "age"
names(d2)[ names(d2) == "Q17" ] = "educ"

# race
var.index = grep( "Q17_", names(d2) )
var.names = c("caucasian", "hisp", "black", "middle.east",
              "pacific.isl", "native.am", "south.asian",
              "east.asian", "southeast.asian"
              )
names(d2)[ var.index ] = var.names


####################### DELETE BORING VARIABLES #######################

# Qualtrics inserts a lot of redundant columns, like indicators for
#  whether subject saw a certain question
boring.ones = c( grep( "Q", names(d2) ), 1:2 )

d3 = d2[ , -boring.ones ]

# sample size in each condition: n=33
# education was the last question, so it indicates completion
table(!is.na(d3$educ), d3$rating.variable)


####################### EXCLUSIONS #######################

exclude = rep("no", dim(d3)[1])

# exclude people who did not reach the last demographic question
#  because stopped taking survey or failed attention check
exclude[ is.na(d3$educ) ] = "didnt.finish.or.attn.check"; table(exclude)

# how many people finished, but failed attention check?
table( d3$excluded)
# n=78 excluded due to attention check

# exclude people who reported technical/comprehension problems
d3$problems
# no problems :)

d4 = d3[ exclude=="no", ]; dim(d4)
# n=66




####################### RESHAPE INTO LONG FORM #######################

# we want a long-form dataset with 81 rows per subject
# each row represents one unique face and subject combo

# reshape wide to long
l = reshape(d4, varying = list(Face=face.names, Emot=emot.names, Time=time.names, Seen=seen.names), 
            v.names=c("main.rating", "emot", "time.spent", "seen.before"),
            #timevar="rating.variable",
            #timevar="survey_code",
           idvar="survey_code",
            direction="long"
)

# check that dimensions make sense
dim(l)
table(duplicated(l$survey_code))
table(l$survey_code)

# compare them
temp.l = l[l$survey_code=="57540092",]
temp.d = d4[d4$survey_code=="57540092",]
# seems to work

# add face variable
l$face = as.factor( rep(1:81, each=dim(d4)[1]) )


####################### RECODE AND CLEAN UP LONG DATASET #######################

l2 = l

# female
l2$female = recode(l2$sex, "1=1; 2=0")

# education
l2$educ = recode(l2$educ, " 1='a.No.HS'; 2='b.HS.only'; 3='c.2.Yr.Coll'; 4='d.4.Yr.Coll'; 
                 5='Post.Grad.Degree' ")

# race vars were originally checkboxes
# recode NAs as 0
race.vars = c("caucasian", "hisp", "black", "middle.east", "pacific.isl", "native.am", 
              "south.asian", "east.asian", "southeast.asian")
l2[,race.vars] = apply( l2[,race.vars], 2, function(x) recode(x, "NA=0") )

# seen face before
l2$seen.face.before =  recode(l2$seen.before, "1=1; 2=0")


# remove unwanted columns
vars.to.toss = c("sex", "seen.before", "LocationLatitude", "LocationLongitude", "LocationAccuracy",
                 "time")
l2 = l2[, !names(l2) %in% vars.to.toss]

# remove face 1 (attn check)
l2 = l2[l2$face!=1,]


####################### ICC OF RATINGS (CLUSTERED BY SUBJECT) #######################

# ICC of human-likeness ratings by subject 
library(ICC)
ICCest(x=survey_code, y=main.rating, data=l2[l2$rating.variable=="human-like",])

# ICC of mechanical-ness ratings by subject 
ICCest(x=survey_code, y=main.rating, data=l2[l2$rating.variable=="mechanical",])





####################### MAKE SHORT VERSION THAT'S JUST THE MEANS PER FACE #######################

# this will have 1 row per FACE
dt = data.table(l)

# put in means by face
s = aggregate( main.rating ~ face, data=l[l$rating.variable=="human-like",], FUN=mean ); names(s)[2] = "mean.human"
s$mean.mech = aggregate( main.rating ~ face, data=l[l$rating.variable=="mechanical",], FUN=mean )$main.rating
s$mean.emot = aggregate( emot ~ face, data=l, FUN=mean )$emot
s$time = aggregate( time.spent ~ face, data=l, FUN=median )$time.spent
s$time.sd = aggregate( time.spent ~ face, data=l, FUN=sd )$time.spent

# make variable for low-emotion faces
s$low.emot = abs(s$mean.emot < 10)

# remove face 1 (attn check)
s = s[s$face!=1,]



####################### WRITE DATA #######################

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/2. Prepped data/1A")

write.csv(l2, paste(Sys.Date(), "exp_1A_long_data_prepped.csv", sep="_") )
write.csv(s, paste(Sys.Date(), "exp_1A_face_means_prepped.csv", sep="_") )





