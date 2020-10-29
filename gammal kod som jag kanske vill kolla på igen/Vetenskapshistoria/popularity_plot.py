import numpy as np
import os
import csv
import matplotlib as mpl
from matplotlib import pyplot as plt

### for plotting nice plots ###
width = 6
height = width / 1.5
fsize = 16
font = {'size': fsize}
mpl.rc('font', **font)
mpl.rc('xtick', labelsize=fsize)
mpl.rc('ytick', labelsize=fsize)
mpl.rc('text', usetex=True)

phrases = ["and"]
case_option = "caseInsensitive"
start_year_option = "1800"
end_year_option = "2004" # data later than 2004 seems off

### fetch and plot frequency data for phrases ###
fig, ax = plt.subplots(figsize=(8,4))
for phrase in phrases:

	### check if data file already exists, if not, run getngrams.py ###
	csv_filename = 'csv files/' + phrase.replace(" ", "") + '-eng_2012-' + start_year_option + '-' + end_year_option + '-3-' + case_option + '.csv'
	if not os.path.isfile(csv_filename):
		os.system("python getngrams.py " + phrase \
			+ " -startYear=" + start_year_option \
			+ " -endYear=" + end_year_option \
			+ " -" + case_option \
			+ " -noprint")

	### read data from file generated by getngrams.py ###
	with open(csv_filename) as csv_file:
		read_csv = csv.reader(csv_file, delimiter=',')

		year = [] # initialize
		popularity = [] # initialize
		for row in read_csv:
			if (row[0].isdigit()):
				year.append(float(row[0]))
				popularity.append(float(row[1]))

	### plot data ###
	ax.plot(year, np.array(popularity), label=phrase)
ax.set_xlabel(r'år')
ax.set_ylabel(r'användningsfrekvens')
ax.set_ylim(bottom=0.0)

### legend & save file ###
plt.tight_layout()
plt.grid()
plt.legend()
filename = f'popularity_plot.pdf'
plt.savefig(filename)
plt.show()