from pyspark.sql import SparkSession
from pyspark.sql.functions import avg, col, desc

spark = SparkSession.builder \
    .appName("Complex SQL Query Example") \
    .getOrCreate()

# sample user ratings for jokes (rows: users, columns: joke ratings)
data = [
    [1, 8.0, 5.5, 7.0, None, 6.0],  
    [2, 6.0, 7.5, 9.0, 8.0, 5.0],   
    [3, 7.0, 6.5, 8.5, 7.5, 7.0],  
    [4, 10.0, None, 9.0, 6.0, 8.5], 
    [5, 8.5, 7.0, None, None, 6.0] 
]
columns = ["UserID", "Joke1", "Joke2", "Joke3", "Joke4", "Joke5"]

ratings_df = spark.createDataFrame(data, columns)

ratings_df.createOrReplaceTempView("ratings")

# query to calculate the average rating for each joke and sort by highest average
sql_query = """
SELECT 
    'Joke1' AS Joke, AVG(Joke1) AS AvgRating FROM ratings UNION ALL
SELECT 
    'Joke2' AS Joke, AVG(Joke2) AS AvgRating FROM ratings UNION ALL
SELECT 
    'Joke3' AS Joke, AVG(Joke3) AS AvgRating FROM ratings UNION ALL
SELECT 
    'Joke4' AS Joke, AVG(Joke4) AS AvgRating FROM ratings UNION ALL
SELECT 
    'Joke5' AS Joke, AVG(Joke5) AS AvgRating FROM ratings
ORDER BY AvgRating DESC
LIMIT 1
"""
top_joke_sql = spark.sql(sql_query)
top_joke_sql.show()

# query to find the user who gave the highest rating to the top joke
joke_name = top_joke_sql.collect()[0]["Joke"]
sql_query_user = f"""
SELECT 
    UserID, {joke_name} AS Rating
FROM 
    ratings
WHERE 
    {joke_name} IS NOT NULL
ORDER BY 
    Rating DESC
LIMIT 1
"""
top_user_sql = spark.sql(sql_query_user)
top_user_sql.show()



# now do this in a single step using DataFrame API

average_ratings_df = ratings_df.select(
    avg("Joke1").alias("Joke1"),
    avg("Joke2").alias("Joke2"),
    avg("Joke3").alias("Joke3"),
    avg("Joke4").alias("Joke4"),
    avg("Joke5").alias("Joke5")
)
average_ratings_df = average_ratings_df.selectExpr(
    "stack(5, 'Joke1', Joke1, 'Joke2', Joke2, 'Joke3', Joke3, 'Joke4', Joke4, 'Joke5', Joke5) as (Joke, AvgRating)"
).orderBy(desc("AvgRating"))
top_joke_df = average_ratings_df.limit(1)
top_joke_df.show()

joke_name_df = top_joke_df.collect()[0]["Joke"]
top_user_df = ratings_df.select("UserID", col(joke_name_df).alias("Rating")) \
    .filter(col("Rating").isNotNull()) \
    .orderBy(desc("Rating")) \
    .limit(1)
top_user_df.show()

spark.stop()
