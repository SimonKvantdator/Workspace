
####################### READ DATA ####################### 

# already used IDs
setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/0. Raw data/Replication 2A-2B")
# used = as.matrix( as.data.frame( read.csv("2015_08_27 Already used worker IDs (UV).csv", header=F) ) )

# Qualtrics data
d1 = read.csv("2015-08-27_qualtrics_minimally_cleaned.csv"); dim(d1)  # n=205

# keep only people who finished
# because Qualtrics logs even people who start the survey but don't finish and complete the MTurk HIT
d2 = d1[ !is.na(d1$Q15), ]; dim(d2)
# 205 people finished



####################### RENAME AND RECODE VARIABLES COMMON TO BOTH EXPERIMENTS #######################

library(car)

# rename some variables
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


# race
var.index = grep( "Q17_", names(d2) )
var.names = c("caucasian", "hisp", "black", "middle.east",
              "pacific.isl", "native.am", "south.asian",
              "east.asian", "southeast.asian"
)
names(d2)[ var.index ] = var.names

# female
d2$female = recode(d2$sex, "1=1; 2=0")

# education
d2$educ = recode(d2$educ, " 1='a.No.HS'; 2='b.HS.only'; 3='c.2.Yr.Coll'; 4='d.4.Yr.Coll'; 
                 5='Post.Grad.Degree' ")

# race vars were originally checkboxes
# recode NAs as 0
race.vars = c("caucasian", "hisp", "black", "middle.east", "pacific.isl", "native.am", 
              "south.asian", "east.asian", "southeast.asian")
d2[,race.vars] = apply( d2[,race.vars], 2, function(x) recode(x, "NA=0") )


# rename id variable
names(d2)[names(d2)=="survey_code"] = "id"

# remove unwanted columns
vars.to.toss = c("sex", "seen.before", "LocationLatitude", "LocationLongitude", "LocationAccuracy",
                 "time")
d2 = d2[, !names(d2) %in% vars.to.toss]



####################### EXCLUDE PEOPLE WITH PROBLEMS ####################### 

# look at reported problems
d2$problems

# exclude people with technical problems/serious confusion
d2$exclude = "no"
d2$exclude[ c(94, 170) ] = "technical.or.confusion.prob"

# exclude n=2
table(d2$exclude, d2$experiment)

d2 = d2[d2$exclude=="no",]

dim(d2)  # n=203



####################### LIKABILITY ####################### 

l = d2[d2$experiment=="likeability",]; dim(l)  # n=105

# main rating variable
var.index = grep( "Q25_", names(l) )
face.names = paste("face.", 1:9, sep="")
names(l)[ var.index ] = face.names

# have you seen face before?
var.index = grep( "Q20.", names(l) )
seen.names = paste("seen.face.", 1:9, sep="")
names(l)[ var.index ] = seen.names

# time spent on each face
var.index = grep( "Q4_3.", names(l) )
time.names = paste("time.face.", 1:9, sep="")
names(l)[ var.index ] = time.names

keepers = c("id", "experiment", "face.1", 
            "face.2", "face.3", "face.4", "face.5", "face.6", 
            "seen.face.1", "seen.face.2", "seen.face.3", "seen.face.4", 
            "seen.face.5", "seen.face.6", 
            "time.face.1",  
            "time.face.2", "time.face.3", 
            "time.face.4", "time.face.5", "time.face.6", 
            "problems", "age", "educ", "caucasian", "hisp", "black", "middle.east", 
            "pacific.isl", "native.am", "south.asian", "east.asian", "southeast.asian", 
            "female")

l2 = l[ , names(l) %in% keepers]

# should be no missing data
library(Amelia)
missmap(l2)


####################### TRUST ####################### 

t = d2[d2$experiment=="trust",]; dim(t)

# main rating variable
var.index = grep( "Q47_", names(t) )
face.names = paste("face.", 1:9, sep="")
names(t)[ var.index ] = face.names

# have you seen face before?
var.index = grep( "Q49.", names(t) )
seen.names = paste("seen.face.", 1:9, sep="")
names(t)[ var.index ] = seen.names

# time spent on each face
var.index = grep( "Q50_3.", names(t) )
time.names = paste("time.face.", 1:9, sep="")
names(t)[ var.index ] = time.names

keepers = c("id", "experiment", "face.1", 
            "face.2", "face.3", "face.4", "face.5", "face.6", 
            "seen.face.1", "seen.face.2", "seen.face.3", "seen.face.4", 
            "seen.face.5", "seen.face.6", 
            "time.face.1",  
            "time.face.2", "time.face.3", 
            "time.face.4", "time.face.5", "time.face.6", 
            "problems", "age", "educ", "caucasian", "hisp", "black", "middle.east", 
            "pacific.isl", "native.am", "south.asian", "east.asian", "southeast.asian", 
            "female")

t2 = t[ , names(t) %in% keepers]

# should be no missing data
library(Amelia)
missmap(t2)
# yes :)


# ### QUICK AND DIRTY!! check out the results!!
# like = colMeans(l2[,3:8], na.rm=TRUE)
# trust = colMeans(t2[,3:8], na.rm=TRUE)
# plot(1:6, like, type="o")
# plot(1:6, trust, type="o")





####################### RESHAPE INTO LONG FORM (LIKEABILITY) #######################

# we want a long-form dataset with 6 rows per subject
# each row represents one unique face and subject combo

face.names = paste("face.", 1:6, sep="")
seen.names = paste("seen.face.", 1:6, sep="")
time.names = paste("time.face.", 1:6, sep="")

# reshape wide to long
l3 = reshape(l2, varying = list(Face=face.names, Time=time.names, Seen=seen.names), 
            v.names=c("main.rating", "time.spent", "seen.before"),
            idvar="id",
            timevar=c("face", "time", "seen"),
            direction="long"
)

# make face variable
l3$face = recode(l3$face, "1='a.Cog'; 2='b.Plastic'; 3='c.Printed'; 4='d.Whiteface'; 5='e.Hanson'; 6='f.Human'")

# check it
dim(l3)  # yes, 6 rows per face now
l2$face.6
l3$main.rating[l3$face=="f.Human"]




####################### RESHAPE INTO LONG FORM (TRUST) #######################

# we want a long-form dataset with 6 rows per subject
# each row represents one unique face and subject combo

face.names = paste("face.", 1:6, sep="")
seen.names = paste("seen.face.", 1:6, sep="")
time.names = paste("time.face.", 1:6, sep="")

# reshape wide to long
t3 = reshape(t2, varying = list(Face=face.names, Time=time.names, Seen=seen.names), 
             v.names=c("main.rating", "time.spent", "seen.before"),
             idvar="id",
             direction="long"
)

# make face variable
t3$face = recode(t3$face, "1='a.Cog'; 2='b.Plastic'; 3='c.Printed'; 4='d.Whiteface'; 5='e.Hanson'; 6='f.Human'")


# check it
dim(t3)  # yes, 6 rows per face now
t2$face.6
t3$main.rating[t3$face=="f.Human"]


####################### WRITE DATA #######################

setwd("~/Dropbox/Uncanny Valley Projects II/2014-12-26 Re-analyses/TIME CAPSULE/Analysis materials/2. Prepped data/Replication 2A-2B")

write.csv(l3, paste(Sys.Date(), "replic_1A_likeability_long_prepped.csv", sep="_") )
write.csv(t3, paste(Sys.Date(), "replic_1B_trust_long_prepped.csv", sep="_") )



