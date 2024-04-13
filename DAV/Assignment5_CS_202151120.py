import pandas as pd
import squarify 
import numpy as np
import matplotlib.pyplot as plt

# Load datasets
df1 = pd.read_csv("Egg_Production_2007_2012.csv")
df2 = pd.read_csv("Milk_Production_2007_2012.csv")

# Merge datasets on the "States/Uts" column
mer_df = pd.merge(df1, df2, on="States/Uts")

# Create a figure and axis for the first nested pie chart
figure1, axis1 = plt.subplots(figsize=(10, 10))

# Labels for the outer and inner pie charts
outer_labels = mer_df.columns[1:6]
inner_labels = mer_df.columns[6:]

# Outer Pie Chart
axis1.pie(mer_df.iloc[0, 1:6], labels=outer_labels, radius=1, autopct='%1.1f%%', startangle=90, wedgeprops=dict(width=0.3, edgecolor='w'))

# Inner Pie Chart
axis1.pie(mer_df.iloc[0, 6:], labels=inner_labels, radius=0.7, autopct='%1.1f%%', startangle=90, wedgeprops=dict(width=0.3, edgecolor='w'))

# Set aspect ratio to be equal for a circular pie chart
axis1.set(aspect="equal")
plt.title('Nested Pie Chart for Production between year 2007 to 2012')
plt.show()

# Create a new figure for the first treemap (Egg Production)
plt.figure(figsize=(10, 8))

# Labels for the treemap
eggLab = mer_df.columns[1:6]
milkLab = mer_df.columns[6:]

# Values for Egg Production
eggVal = mer_df.iloc[0, 1:6].tolist()

# Plot the treemap for Egg Production
squarify.plot(sizes=eggVal, label=eggLab, color=[plt.cm.tab20c(i/len(eggVal)) for i in range(len(eggVal))], alpha=0.7, pad=True, axis1=plt.gca())
plt.axis('off')
plt.title('TreeMap for Egg Production between year 2007 to 2012)')
plt.show()

# Create a new figure for the second treemap (Milk Production)
plt.figure(figsize=(10, 8))

# Values for Milk Production
milkVal = mer_df.iloc[0, 6:].tolist()

# Plot the treemap for Milk Production
squarify.plot(sizes=milkVal, label=milkLab, color=[plt.cm.tab20c(i/len(milkVal)) for i in range(len(milkVal))], alpha=0.7, pad=True, axis1=plt.gca())
plt.axis('off')
plt.title('TreeMap for Milk Production between year 2007 to 2012)')
plt.show()
