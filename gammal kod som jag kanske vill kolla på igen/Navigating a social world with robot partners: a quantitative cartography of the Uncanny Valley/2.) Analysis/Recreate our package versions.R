
################################# CREATING SNAPSHOT #################################

# you do NOT run this section

# setwd("~/Dropbox/a.Uncanny Valley Projects II/2014-12-26 Re-analyses/Daz shared folder (UV)/TIME CAPSULE for OSF/2.) Analysis")
# 
# library(pkgsnap)
# snap(to = paste(Sys.Date(), "packages.csv", sep="_") , recommended = FALSE)


################################# READ IN DATA #################################

# if you don't already use packrat, run these
install.packages("packrat")
install.packages("devtools")
devtools::install_github("rstudio/packrat")

# recreate MM's libraries
snapshot = read.csv("2017-02-19_packages.csv")
restore(snapshot)

# for more info: https://github.com/MangoTheCat/pkgsnap