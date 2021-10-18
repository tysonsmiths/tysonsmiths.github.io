from datetime import datetime
from pymongo import MongoClient, ASCENDING, DESCENDING
from bson.objectid import ObjectId
import math
import pandas as pd


class AnimalShelter(object):
    """ CRUD operations for Animal collection in MongoDB """

    loaded_data = None
    loaded_read_info = None
    loaded_total_count = None

    def __init__(self):
        # Initializing the MongoClient. This helps to
        # access the MongoDB databases and collections.
        self.client = MongoClient('mongodb://localhost:27017/')
        self.database = self.client["aac"]

    def create(self, data):
        if data is not None:
            try:
                self.database.animals.insert_one(
                    data)  # data should be a dictionary
                return True
            except:
                return False
        else:
            raise Exception("Nothing to save because data parameter is empty")

    def readWithPagination(self, page_index, page_size, load_pages_amount, query, sort=None, df_sanitation_fn=None):
        page_start = page_index * page_size
        load_size = load_pages_amount * page_size
        load_index = math.floor(page_start / load_size)
        load_start = load_index * load_size

        df_records, total_count_before_limit_and_skip = self.read(
            query, sort=sort, skip=load_start, limit=load_size, include_total_count_before_limit_and_skip=True)


        if (df_sanitation_fn is not None):
          df_records = df_sanitation_fn(df_records)

        if (sort is not None):
            df_records = df_records.sort_values(
                by=sort['column_id'], ascending=True if sort['direction'] == 'asc' else False)

        page_start_in_load = page_start % load_size
        page_end_in_load = page_start_in_load + page_size
        return df_records[page_start_in_load:page_end_in_load], total_count_before_limit_and_skip

    def read(self, query, sort=None, skip=0, limit=None, include_total_count_before_limit_and_skip=False):
        read_info = {
            'query': query,
            'sort': sort,
            'skip': skip,
            'limit': limit,
        }

        # A simple cache that if the query is the same as the last one, return the same
        # data as the last query
        if (self.shouldReturnLoadedData(read_info)):
            return self.getLoadedData(), self.loaded_total_count

        cursor = self.database.animals.find(query, {"_id": False})
        total_count_before_limit_and_skip = cursor.count()
        self.loaded_total_count = total_count_before_limit_and_skip

        id_sort = ("_id", ASCENDING)
        if (sort is not None):
            column_sort = (
                sort['column_id'], ASCENDING if sort['direction'] == 'asc' else DESCENDING)
            cursor.sort([column_sort, id_sort])
        else:
            cursor.sort([id_sort])

        if (not skip == 0):
            cursor.skip(skip)

        if (limit is not None):
            cursor.limit(limit)

        df_records = pd.DataFrame.from_records(cursor)
        self.setLoadedData(df_records, read_info)

        if (include_total_count_before_limit_and_skip):
          return df_records, total_count_before_limit_and_skip

        return df_records

    def getLoadedData(self):
        return self.loaded_data

    def setLoadedData(self, data, loaded_read_info):
        self.loaded_data = data
        self.loaded_read_info = loaded_read_info

    def shouldReturnLoadedData(self, read_info):
        return self.loaded_data is not None and read_info == self.loaded_read_info

    def update(self, query, data):
        if query is not None and data is not None:
            try:
                # query and data should be a dictionaries
                return self.database.animals.update_one(query, data)
            except Exception as e:
                return str(e)
        else:
            raise Exception(
                "Nothing to update because query and or data parameters are empty")

    def delete(self, query):
        if query is not None:
            try:
                # query should be a dictionary
                return self.database.animals.delete_one(query)
            except Exception as e:
                return str(e)
        else:
            raise Exception(
                "Nothing to delete because query parameter is empty")
