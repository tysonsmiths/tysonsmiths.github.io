import dash
import dash_leaflet as dl
from dash import dcc, html, dash_table
import plotly.express as px
from dash import dash_table
from dash.dependencies import Input, Output


import numpy as np
import pandas as pd
from aac import AnimalShelter

# How many rows on a page
PAGE_SIZE = 10
# How many pages to load at a time
LOAD_PAGES_AMOUNT = 10

# ###########################
# # Data Manipulation / Model
# ###########################
shelter = AnimalShelter()

def getQueryForRescueType(rescue_type):
    if rescue_type == 'water':
        return {
            '$and': [
                {
                    "breed": {
                        "$in": [
                            'Labrador Retriever Mix',
                            'Chesapeake Bay Retriever',
                            'Newfoundland',
                        ],
                    },
                },
                {
                    'sex_upon_outcome': 'Intact Female',
                },
                {
                    'age_upon_outcome_in_weeks': {'$gte': 26}
                },
                {
                    'age_upon_outcome_in_weeks': {'$lte': 156}
                },
            ],
        }
    elif rescue_type == 'mountain':
        return {
            '$and': [
                {
                    "breed": {
                        "$in": [
                            'German Shepherd',
                            'Alaskan Malamute',
                            'Old English Sheepdog',
                            'Siberian Husky',
                            'Rottweiler',
                        ],
                    },
                },
                {
                    'sex_upon_outcome': 'Intact Male',
                },
                {
                    'age_upon_outcome_in_weeks': {'$gte': 26}
                },
                {
                    'age_upon_outcome_in_weeks': {'$lte': 156}
                },
            ],
        }
    elif rescue_type == 'disaster':
        return {
            '$and': [
                {
                    "breed": {
                        "$in": [
                            'Doberman Pinscher',
                            'German Shepherd',
                            'Golden Retriever',
                            'Bloodhound',
                            'Rottweiler',
                        ],
                    },
                },
                {
                    'sex_upon_outcome': 'Intact Male',
                },
                {
                    'age_upon_outcome_in_weeks': {'$gte': 20}
                },
                {
                    'age_upon_outcome_in_weeks': {'$lte': 300}
                },
            ],
        }
    return {}


#########################
# Dashboard Layout / View
#########################
app = dash.Dash(__name__)

app.layout = html.Div([
    html.Div(id='hidden-div', style={'display': 'none'}),
    html.Center(html.B(html.H1('Animal Shelter Dashboard'))),
    html.Center(html.H3('Created by Tyson Smith')),
    html.Hr(),
    dcc.RadioItems(
        id='radio-id',
        options=[
            {'label': 'Water Rescue', 'value': 'water'},
            {'label': 'Mountain Rescue', 'value': 'mountain'},
            {'label': 'Disaster Rescue', 'value': 'disaster'},
            {'label': 'Reset', 'value': ''},
        ],
    ),
    dash_table.DataTable(
        id='datatable-id',
        page_size=10,
        sort_action="custom",
        sort_mode='single',
        page_current=0,
        page_action='custom',
    ),
    html.Br(),
    html.Hr(),
    html.Div(
        style={'min-width': '1600px', 'width': '100vw'},
        children=[
            html.Div(
                style={'float': 'left'},
                children=[
                    html.H3('Breed Frequency Pie Chart for Current Table View'),
                    dcc.RadioItems(
                        id='pie-radio-id',
                        options=[
                            {'label': 'All data', 'value': 1},
                            {'label': 'Current Table View', 'value': 0},
                        ],
                        style={'display': 'none'},
                        value=0,
                    ),
                    html.Div(
                        id='pie-id'
                    ),
                ]
            ),
            html.Div(
                style={'float': 'left'},
                children=[
                    html.H3(
                        'Locations Mapped for Animals in the Current Table View'),
                    html.Div(
                        id='map-id',
                    ),
                ]
            ),
        ]
    )

])

#############################################
# Interaction Between Components / Controller
#############################################

# Helper ano
def dfSanitationFn(df):
    df['name'] = df['name'].astype(str)
    return df

@app.callback(
    [
        Output('datatable-id', 'data'),
        Output('datatable-id', 'columns'),
        Output('datatable-id', 'page_size'),
        Output('datatable-id', 'page_count'),
    ],
    [
        Input('radio-id', 'value'),
        Input('datatable-id', 'sort_by'),
        Input('datatable-id', 'page_current')
    ]
)
def update_table(rescue_type, sort_by, page_current):
    sort = sort_by[0] if sort_by else None
    query = getQueryForRescueType(rescue_type)

    
    df_records, total_table_count = shelter.readWithPagination(
        page_current, PAGE_SIZE, LOAD_PAGES_AMOUNT, query, sort=sort, df_sanitation_fn=dfSanitationFn)

    data = df_records.to_dict('records')
    columns = [
        {"name": i, "id": i, "deletable": False, "selectable": True} for i in df_records.columns[1:]
    ]

    return data, columns, PAGE_SIZE, total_table_count

# This callback will highlight a row on the data table when the user selects it
@app.callback(
    Output('datatable-id', 'style_data_conditional'),
    [Input('datatable-id', 'selected_columns')]
)
def update_styles(selected_columns):
    if (not selected_columns):
        return None

    return [{
        'if': {'column_id': i},
        'background_color': '#D2F3FF'
    } for i in selected_columns]


@app.callback(
    Output('pie-id', 'children'),
    [Input('datatable-id', "derived_viewport_data"),
     Input('datatable-id', "data"),
     Input('pie-radio-id', 'value')])
def update_pie(viewData, fullData, useFullData):
    data = fullData if useFullData else viewData
    if (not data):
        return None

    df_sizes = pd.DataFrame.from_dict(data).groupby('breed').size()
    total = df_sizes.sum()
    df = df_sizes.reset_index(name='count')
    df.loc[df['count']/total < .01, 'breed'] = 'Other breeds'
    fig = px.pie(df, values='count', names='breed',
                 title="Total Breeds: " + str(df_sizes.count()))
    return dcc.Graph(
        style={'width': '600px', 'height': '500px'},
        figure=fig
    )


@app.callback(
    Output('map-id', 'children'),
    [Input('datatable-id', "derived_viewport_data")])
def update_map(viewData):
    dff = pd.DataFrame.from_dict(viewData)
    markers = []
    for index, row in dff.iterrows():
        info_html = []
        for key, value in row.iloc[1:].to_dict().items():
            info_html.append(key + ': ' + (str(value) or 'N/A'))
            info_html.append(html.Br())

        marker = dl.Marker(position=[row.location_lat, row.location_long], children=[
            dl.Tooltip(row.color + ' ' + row.breed + ' ' + row.animal_type),
            dl.Popup([
                html.H1((row.animal_id or 'NO ID') + ': ' +
                        (str(row.to_dict()['name']) or 'UNNAMED')),
                html.P(info_html)
            ])
        ])
        markers.append(marker)

    return [                                            # Austin TX is at [30.75,-97.48]
        dl.Map(style={'width': '1000px', 'height': '500px'}, center=[30.75, -97.48], zoom=9, children=[
            dl.TileLayer(id="base-layer-id")] + markers
        )
    ]


if __name__ == "__main__":
    app.run_server(debug=True)
