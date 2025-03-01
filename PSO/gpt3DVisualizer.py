import plotly.express as px
import pandas as pd

# Sample data
data = {'x': [1, 2, 3, 4, 5],
        'y': [6, 7, 2, 4, 5],
        'z': [2, 3, 7, 8, 2],
        'color': ['A', 'B', 'A', 'B', 'A']}
df = pd.DataFrame(data)

# Create the 3D scatter plot
fig = px.scatter_3d(df, x='x', y='y', z='z',
                    color='color', symbol='color',
                    title='3D Scatter Plot Example')
fig.show()