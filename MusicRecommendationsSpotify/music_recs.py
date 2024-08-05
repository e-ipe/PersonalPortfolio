import os
import numpy as np
import pandas as pd

from yellowbrick.target import FeatureCorrelation
import matplotlib.pyplot as plt
import plotly.express as px

class MusicRecs():

    def __init__(self):
        self.data, self.genre_data, self.year_data = self.setUpClass()

    @staticmethod
    def setUpClass():
        data = pd.read_csv("./data/data.csv")
        print("\n\tData: ")
        print(data.info())
        genre_data = pd.read_csv("./data/data_by_genres.csv")
        print("\n\tGenre Data: ")
        print(genre_data.info())
        year_data = pd.read_csv("./data/data_by_year.csv")
        print("\n\tYear Data: ")
        print(year_data.info())
        return data, genre_data, year_data

    @staticmethod
    def visualize(data):
        # these are some values in data
        feature_names = ['acousticness', 'danceability', 'energy', 'instrumentalness',
        'liveness', 'loudness', 'speechiness', 'tempo', 'valence','duration_ms','explicit','key','mode','year']
        X, y = data[feature_names], data['popularity'] # X is the df with just those columns, y is just popularity
        features = np.array(feature_names) # list of feature names that we initialized, printed as an array
        visualizer = FeatureCorrelation(labels=features) # opens up another window with the visual
        visualizer.fit(X, y)  
        visualizer.show()
         # Visualizing the data with FeatureCorrelation shows that year has a high correlation to the dependent variable
        return
    
    @staticmethod
    def get_decade(year):
        # Splitting the data into decades to see how many songs are in each decade
        period_start = int(year/10) * 10
        decade = '{}s'.format(period_start)
        return decade
    
    def visualize_decades(self, data): # needs self because it uses get_decade
        data['decade'] = data['year'].apply(self.get_decade) # apply get_decade to get another column
        fig = px.histogram( # dash application!
            data,
            x='decade',
            title='Number of Songs by Decade',
            labels={'decade': 'Decade', 'count': 'Number of Songs'}
        )
        fig.show()
        return
    
    @staticmethod
    def visualize_sound_features():
        sound_features = ['acousticness', 'danceability', 'energy', 'instrumentalness', 'liveness', 'valence']
        fig = px.line(year_data, x='year', y=sound_features) # a line using all the features listed
        fig.show()
        return
    
    @staticmethod
    def visualize_genres():
        top10_genres = genre_data.nlargest(10, 'popularity') # top 10 rows of genre_data based on popularity
        fig = px.bar(top10_genres, x='genres', y=['valence', 'energy', 'danceability', 'acousticness'], barmode='group') # only y is used when plotted
        fig.show()
        return

    def clustering():
        return

    def recommendations():
        return

if __name__ == '__main__':
    recs = MusicRecs()
    data, genre_data, year_data = recs.setUpClass()
    recs.visualize(data)
    recs.visualize_decades(data)
    recs.visualize_sound_features()
    recs.visualize_genres()