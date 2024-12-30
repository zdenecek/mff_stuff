import redis 

# SETUP
# pip install redis-py
# docker run -d --name redis-stack-server -p 6379:6379 redis/redis-stack-server:latest

redis_client = redis.StrictRedis(host='localhost', port=6379, decode_responses=True)

jokes = {
    1: {"text": "Why don't scientists trust atoms? Because they make up everything!", "rating": 8.5, "category": "Science"},
    2: {"text": "Why did the chicken join a band? Because it had the drumsticks!", "rating": 7.2, "category": "Animal"},
    3: {"text": "What do you call fake spaghetti? An impasta!", "rating": 8.0, "category": "Food"},
    4: {"text": "Why don't skeletons fight each other? They don't have the guts.", "rating": 6.9, "category": "Skeleton"},
}

user_ratings = {
    "user1": {1: 9, 2: 7},
    "user2": {1: 8, 3: 9},
    "user3": {2: 6, 3: 7, 4: 8},
}

# store jokes in redis as hashes
for joke_id, details in jokes.items():
    redis_client.hset(f"joke:{joke_id}", mapping=details)

# store joke ratings in a sorted set
for joke_id, details in jokes.items():
    redis_client.zadd("joke_ratings", {joke_id: details["rating"]})

# store user ratings in hashes and update a list of active users
for user, ratings in user_ratings.items():
    for joke_id, rating in ratings.items():
        redis_client.hset(f"user:{user}:ratings", joke_id, rating)
    redis_client.lpush("active_users", user)

# store joke categories in sets

for joke_id, details in jokes.items():
    redis_client.sadd(f"category:{details['category']}", joke_id)

# Ukázky operací 

# 1. joke by ID
def get_joke(joke_id):
    joke = redis_client.hgetall(f"joke:{joke_id}")
    return joke

print("Joke 1 Details:", get_joke(1))

# 2. top-rated jokes
def get_top_jokes(n=3):
    top_jokes = redis_client.zrevrange("joke_ratings", 0, n - 1, withscores=True)
    return [(int(joke_id), score) for joke_id, score in top_jokes]

print("Top 3 Jokes:", get_top_jokes())

# 3. all jokes in a category
def get_jokes_by_category(category):
    joke_ids = redis_client.smembers(f"category:{category}")
    jokes_in_category = [get_joke(joke_id) for joke_id in joke_ids]
    return jokes_in_category

print("Jokes in 'Science' Category:", get_jokes_by_category("Science"))

# 4. active users
def get_active_users(n=5):
    return redis_client.lrange("active_users", 0, n - 1)

print("Active Users:", get_active_users())

# 5. get user ratings
def get_user_ratings(user):
    return redis_client.hgetall(f"user:{user}:ratings")

print("Ratings by user1:", get_user_ratings("user1"))

# 6. rating updates
def update_joke_rating(joke_id, new_rating):
    joke_key = f"joke:{joke_id}"
    old_rating = float(redis_client.hget(joke_key, "rating"))
    updated_rating = (old_rating + new_rating) / 2  # example calculation
    redis_client.hset(joke_key, "rating", updated_rating)
    
    redis_client.zadd("joke_ratings", {joke_id: updated_rating})

update_joke_rating(1, 9.0)
print("Updated Top Jokes:", get_top_jokes())


# OUTPUT
# Joke 1 Details: {'category': 'Science', 'rating': '8.5', 'text': "Why don't scientists trust atoms? Because they make up everything!"}
# Top 3 Jokes: [(1, 8.5), (3, 8.0), (2, 7.2)]
# Jokes in 'Science' Category: [{'category': 'Science', 'rating': '8.5', 'text': "Why don't scientists trust atoms? Because they make up everything!"}]
# Active Users: ['user3', 'user2', 'user1', 'user3', 'user2']
# Ratings by user1: {'1': '9', '2': '7'}
# Updated Top Jokes: [(1, 8.75), (3, 8.0), (2, 7.2)]
