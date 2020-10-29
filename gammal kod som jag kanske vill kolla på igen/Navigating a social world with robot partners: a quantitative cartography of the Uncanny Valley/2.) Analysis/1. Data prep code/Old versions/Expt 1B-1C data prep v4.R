
####################### READ DATA ####################### 

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/0. Raw data/1B-1C")
d1 = read.csv("2015_05-04_qualtrics_raw_no_header.csv"); dim(d1)  # 737

# name survey variable to match the one in MTurk file
d1$survey_code = paste("QTRX", d1$survey_code, sep="")

# keep only people who finished
# because Qualtrics logs even people who start the survey but don't finish and complete the MTurk HIT
d2 = d1[ !is.na(d1$Q16), ]; dim(d2)
# 686 people finished

# read face data from Expt 1A
setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/2. Prepped data/1A")
s = read.csv("2015-06-17_exp_1A_face_means_prepped.csv")
s=s[,-1]  # remove superfluous column
dim(s)  # 80

# read long data from Expt 1A
# on humanness and mechanicalness
setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/2. Prepped data/1A")
h = read.csv("2015-06-17_exp_1A_long_data_prepped.csv"); dim(h)  # 5280

library(lme4)
library(data.table)


####################### RENAME AND RECODE VARIABLES COMMON TO BOTH EXPERIMENTS #######################

# rename some variables
names(d2)[ names(d2) == "Q18" ] = "problems"
names(d2)[ names(d2) == "Q17.1" ] = "too.many.faces"
names(d2)[ names(d2) == "Q16" ] = "sex"
names(d2)[ names(d2) == "Q15" ] = "age"
names(d2)[ names(d2) == "Q17.2" ] = "educ"

# race
var.index = grep( "Q17_", names(d2) )
var.names = c("caucasian", "hisp", "black", "middle.east",
              "pacific.isl", "native.am", "south.asian",
              "east.asian", "southeast.asian"
)
names(d2)[ var.index ] = var.names

# female
d2$female = recode(d2$sex, "1=1; 2=0")

# too many faces
d2$too.many.faces = recode(d2$too.many.faces, "1=1; 2=0")

# education
d2$educ = recode(d2$educ, " 1='a.No.HS'; 2='b.HS.only'; 3='c.2.Yr.Coll'; 4='d.4.Yr.Coll'; 
                 5='Post.Grad.Degree' ")

# race vars were originally checkboxes
# recode NAs as 0
race.vars = c("caucasian", "hisp", "black", "middle.east", "pacific.isl", "native.am", 
              "south.asian", "east.asian", "southeast.asian")
d2[,race.vars] = apply( d2[,race.vars], 2, function(x) recode(x, "NA=0") )

# remove unwanted columns
vars.to.toss = c("sex", "seen.before", "LocationLatitude", "LocationLongitude", "LocationAccuracy",
                 "time")
d2 = d2[, !names(d2) %in% vars.to.toss]


####################### EXCLUDE PEOPLE WITH PROBLEMS ####################### 

# look at reported problems
d2$problems

# exclude people with technical problems/serious confusion
d2$exclude = "no"
d2$exclude[ c(310, 424, 489, 495, 502, 533, 567, 582, 619, 651) ] = "technical.or.confusion.prob"

# exclude n=10
table(d2$exclude, d2$experiment)

d2 = d2[d2$exclude=="no",]



####################### SPLIT INTO 2 EXPERIMENTS: LIKEABILITY (L) AND TRUST (T) ####################### 

t1 = d2[ d2$experiment=="trust", ]; dim(t1)  # n=334

l1 = d2[ d2$experiment=="likeability", ]; dim(l1)  # n=342


####################### RENAME VARIABLES (L) ####################### 

l2=l1

# wager variable
var.index = grep( "Q25.", names(l2) )
face.names = paste("face.", 2:81, sep="")
names(l2)[ var.index ] = face.names

# have you seen face before
var.index = grep( "Q20.", names(l2) )
seen.names = paste("seen.face.", 2:81, sep="")
names(l2)[ var.index ] = seen.names

# time spent on each face
var.index = grep( "Q4_3.", names(l2) )
time.names = paste("time.face.", 2:81, sep="")
names(l2)[ var.index ] = time.names


####################### RENAME VARIABLES (T) ####################### 

t2=t1

# trust variable
var.index = grep( "Q47.", names(t2) )
face.names = paste("face.", 2:81, sep="")
names(t2)[ var.index ] = face.names

# have you seen face before
var.index = grep( "Q49.", names(t2) )
seen.names = paste("seen.face.", 2:81, sep="")
names(t2)[ var.index ] = seen.names

# time spent on each face
var.index = grep( "Q50_3.", names(t2) )
time.names = paste("time.face.", 2:81, sep="")
names(t2)[ var.index ] = time.names


####################### DELETE BORING VARIABLES #######################

# Qualtrics automatically generates a lot of redundant variables
# e.g., a million indicators for whether a S saw a certain question

boring.ones = c( grep( "Q", names(t2) ), 1:10 )
t3 = t2[ , -boring.ones ]

boring.ones = c( grep( "Q", names(l2) ), 1:10 )
l3 = l2[ , -boring.ones ]



####################### RESHAPE INTO LONG FORM (LIKEABILITY) #######################

# we want a long-form dataset with 80 rows per subject
# each row represents one unique face and subject combo

# reshape wide to long
l4 = reshape(l3, varying = list(Face=face.names, Time=time.names, Seen=seen.names), 
            v.names=c("main.rating", "time.spent", "seen.before"),
            idvar="survey_code",
            direction="long"
)


# check it
dim(l4)
table(duplicated(l4$survey_code))
table(l4$survey_code)

# compare them in a simple example to make sure above code worked
#temp.1 = l4[l4$survey_code=="QTRX92136871",]
#temp.2 = l3[l3$survey_code=="QTRX92136871",]
# works

# add face variable
# starts at index 2 since face 1 (attn check) is not in this study
l4$face = as.factor( rep(2:81, each=dim(l3)[1]) )
length(unique(l4$face))  # yes, 80 rows, 1 for each face


####################### RESHAPE INTO LONG FORM (TRUST) #######################

# we want a long-form dataset with 80 rows per subject
# each row represents one unique face and subject combo

# reshape wide to long
t4 = reshape(t3, varying = list(Face=face.names, Time=time.names, Seen=seen.names), 
             v.names=c("main.rating", "time.spent", "seen.before"),
             #timevar="rating.variable",
             #timevar="survey_code",
             idvar="survey_code",
             direction="long"
)


dim(t4)
table(duplicated(t4$survey_code))
table(t4$survey_code)

# compare them
#temp.1 = l4[l4$survey_code=="QTRX92136871",]
#temp.2 = l3[l3$survey_code=="QTRX92136871",]
# seems to work...

# add face variable
t4$face = as.factor( rep(2:81, each=dim(t3)[1]) )



####################### MERGE INTO SHORT DATASET (FACE AVERAGES) #######################

##### Unadjusted Means (Primary) #####
dt = data.table(l4)
s$like.mean = aggregate( main.rating ~ face, data=l4, FUN=mean )$main.rating # the NA is for Face 1, the attention-check
s$like.sd = aggregate( main.rating ~ face, data=t4, FUN=sd )$main.rating  # the NA is for Face 1, the attention-check

dt = data.table(t4)
s$trust.mean = aggregate( main.rating ~ face, data=t4, FUN=mean )$main.rating  # the NA is for Face 1, the attention-check
s$trust.sd = aggregate( main.rating ~ face, data=t4, FUN=sd )$main.rating # the NA is for Face 1, the attention-check

# make composite measure of human-likeness that uses both human and mech measures
s$composite = s$mean.human - s$mean.mech

# centered composite to give mean 0
s$composite2 = s$composite - mean(s$composite)

# absolute distance of face's mechano-humanness score from the H-M score that defines
#  the UV nadir for likeability
# this came from the analysis script for Exp 1B-1C and then was brought back here
nadir = 36.4841
s$nadir.dist = abs(s$composite - nadir)



##### "Adjusted" Means (from Mixed Model) - Sensitivity Analysis #####

# not doing it for humanness composite because every S judged every face,
#  so S random effect is basically taken care of

# likeability
m = lmer(main.rating ~ as.factor(face) + (1|survey_code), data=l4)
coef = coef(summary(m))
int = coef[1,1]  # intercept
face.adj.means = c(int, int + coef[2:80,1] )  # face coefficients
s$like.adj.mean = face.adj.means
plot(s$like.mean, s$like.adj.mean)  # very similar, as expected

# trust
m = lmer(main.rating ~ as.factor(face) + (1|survey_code), data=t4)
coef = coef(summary(m))
int = coef[1,1]  # intercept
face.adj.means = c(int, int + coef[2:80,1] )  # face coefficients
s$trust.adj.mean = face.adj.means
plot(s$trust.mean, s$trust.adj.mean)  # similar, as expected

# add variable for emotional valence (positive or negative)
# coded as NA for low-emotion faces
s$emot.valence = NA
s$emot.valence[s$mean.emot < -10] = "neg"
s$emot.valence[s$mean.emot > 10] = "pos"

# order by the composite score
s = s[ order(s$composite), ]


####################### MERGE FACE AVERAGES INTO LONG DATASETS #######################

# NOT CURRENTLY USING
# THE BELOW CODE DOES NOT WORK BECAUSE THE VARIABLE CALLED "TIME" IN S 
#  ATTEMPTS TO MATCH TO THE VARIABLE CALLED "TIME.SPENT" IN L4 AND T4

#l5 = merge(l4, fake, on.x="face", on.y="face")
#t5 = merge(t4, s, on.x="face", on.y="face")

l5=l4
t5=t4

####################### RENAME VARIABLES #######################

names(l5)[names(l5)=="survey_code"] = "id"
names(t5)[names(t5)=="survey_code"] = "id"




####################### REMOVE USELESS OBSERVATIONS #######################

# observations where rating is NA (because subject wasn't assigned to see that face)
l5 = l5[ !is.na(l5$main.rating), ]; dim(l5)  # 5093
t5 = t5[ !is.na(t5$main.rating), ]; dim(t5)  # 5008


####################### WRITE DATA #######################

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/2. Prepped data/1B-1C")

write.csv(l5, paste(Sys.Date(), "expt_1B_likeability_long_prepped.csv", sep="_") )
write.csv(t5, paste(Sys.Date(), "expt_1C_trust_long_prepped.csv", sep="_") )
write.csv(s, paste(Sys.Date(), "expt_1B_1C_face_means.csv", sep="_") )



