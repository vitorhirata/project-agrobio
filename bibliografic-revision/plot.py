import numpy as np
import matplotlib.pyplot as plt
import geopandas as gpd
import pandas as pd
from shapely.geometry import Point, Polygon
from matplotlib.lines import Line2D

df = pd.read_csv("csv/data_revision.csv", sep=";")
geometry = [Point(xy) for xy in zip(df.Long, df.Lat)]
crs = {'init': 'epsg:4326'}
gdf = gpd.GeoDataFrame(df, crs=crs, geometry=geometry)
world = gpd.read_file(gpd.datasets.get_path('naturalearth_lowres'))

fig, ax = plt.subplots(1)
fig.set_dpi(1000.0)
base = world.plot(ax=ax, color='white', edgecolor='black')
gdf[gdf.Specie == 'Manioc'].plot(ax=base, color='k', markersize=25, marker='^', facecolors='none')
gdf[gdf.Specie == 'Maiz'].plot(ax=base, color='k', markersize=20, marker='o', facecolors='none')
#gdf[gdf.Specie == 'Potato'].plot(ax=base, color='green', markersize=7)
_ = ax.axis('off')

leg = [Line2D([0], [0], marker='o', color='w', label='Maiz', markeredgecolor='k', markerfacecolor='none', markersize=6),
        #Line2D([0], [0], marker='o', color='w', label='Potato', markerfacecolor='g', markersize=5),
        Line2D([0], [0], marker='^', color='w', label='Manioc', markeredgecolor='k', markerfacecolor='none', markersize=7)]
ax.legend(handles=leg, loc=(0.1, 0.4), fontsize = 'x-large')
plt.show()
