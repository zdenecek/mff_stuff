#!/usr/bin/env python3
import sys
from pyspark.sql import SparkSession
from pyspark.sql.types import StructType, StructField, StringType
from pyspark.sql.functions import col, substring, count as _count, sum as _sum

spark = SparkSession.builder.appName("NameCollisionsByRegion") \
    .config("spark.serializer", "org.apache.spark.serializer.KryoSerializer") \
    .config("spark.sql.adaptive.enabled", "true") \
    .getOrCreate()
sc = spark.sparkContext
sc.getConf().set("spark.sql.shuffle.partitions", sc.defaultParallelism)

schema = StructType([
    StructField("first", StringType(), True),
    StructField("last",  StringType(), True),
    StructField("other", StringType(), True),
    StructField("zipc",  StringType(), True)
])

input_path = sys.argv[1]

df = spark.read.schema(schema).csv(input_path).select("first", "last", "zipc")
df = df.withColumn("region", substring(col("zipc"), 1, 1).cast("int"))

region_totals = (
    df.groupBy("region", "first", "last")
      .agg(_count("*").alias("cnt"))
      .filter(col("cnt") >= 2)
      .groupBy("region")
      .agg(_sum("cnt").alias("total"))
      .orderBy("region")
)

rows = region_totals.collect()
with open("output.csv", "w") as out:
    for r in rows:
        out.write(f"{r.region},{r.total}\n")

spark.stop()
