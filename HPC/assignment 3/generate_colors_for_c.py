from matplotlib import cm

cmap = cm.get_cmap('viridis')
for i in range(12):
    rgba = cmap(i / 12.)
    print(f'X[{i}] = "{int(10.0 * rgba[0])} {int(10.0 * rgba[1])} {int(10.0 * rgba[2])}\\n"; \\')  # (0.99807766255210428, 0.99923106502084169, 0.74602077638401709, 1.0)
