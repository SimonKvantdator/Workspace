import matplotlib as mpl
import matplotlib.pyplot as plt
import pandas as pd
from pytrends.request import TrendReq

### for plotting nice plots ###
width = 6
height = 4
fsize = 16
font = {'size': fsize}
mpl.rc('font', **font)
mpl.rc('xtick', labelsize=fsize)
mpl.rc('ytick', labelsize=fsize)
mpl.rc('text', usetex=False)

terms = ['color', 'colour']
pytrend = TrendReq()
pytrend.build_payload(kw_list=terms)

data = pytrend.interest_over_time()

### write output to text file ###
text_file = open("google_trends_plotter_output.txt", "w")
# text_file.write(data)
text_file.close()

### plot trends ###
fig, ax = plt.subplots(figsize=(width, height))
for term in terms:
    print(data.to_dict()[term])
    ax.plot(data[term].to_numpy())
# plt.show()


