# healthcheck
curl -v http://localhost:10011/ping

# list buckets
curl http://localhost:10011/riak?buckets=true | python3 -mjson.tool

# create jokes bucket
curl -X PUT http://localhost:10011/riak/tomisz_jokes \
     -H "Content-Type: application/json" \
     -d '{"props" : { "n_val" : 3 } }'

# insert all jokes
curl -i -H "Content-Type: application/json" \
     -d '{"text": "Why don'\''t scientists trust atoms? Because they make up everything!", "rating": 8.5, "category": "Science"}' \
     http://localhost:10011/riak/tomisz_jokes/1

curl -i -H "Content-Type: application/json" \
     -d '{"text": "Why did the chicken join a band? Because it had the drumsticks!", "rating": 7.2, "category": "Animal"}' \
     http://localhost:10011/riak/tomisz_jokes/2

curl -i -H "Content-Type: application/json" \
     -d '{"text": "What do you call fake spaghetti? An impasta!", "rating": 8.0, "category": "Food"}' \
     http://localhost:10011/riak/tomisz_jokes/3

# retrieve a specific joke
curl http://localhost:10011/riak/tomisz_jokes/1 | python3 -mjson.tool

# list all jokes in the bucket
curl http://localhost:10011/riak/tomisz_jokes?keys=true | python3 -mjson.tool

# update a joke
curl -i -X PUT -H "Content-Type: application/json" \
     -d '{"text": "Why don'\''t scientists trust atoms? Because they make up everything! Updated version.", "rating": 9.0, "category": "Science"}' \
     http://localhost:10011/riak/tomisz_jokes/1

# links between jokes and categories
curl -H "Content-Type: text/plain" \
     -H 'Link: </riak/tomisz_jokes/1>; riaktag="belongs_to"' \
     -d "Category: Science" \
     http://localhost:10011/riak/tomisz_categories/Science

curl -H "Content-Type: text/plain" \
     -H 'Link: </riak/tomisz_jokes/2>; riaktag="belongs_to"' \
     -d "Category: Animal" \
     http://localhost:10011/riak/tomisz_categories/Animal

# retrieve jokes linked to category
curl http://localhost:10011/riak/tomisz_categories/Science | python3 -mjson.tool

# delete joke
curl -i -X DELETE http://localhost:10011/riak/tomisz_jokes/1

# list  jokes after deletion
curl http://localhost:10011/riak/tomisz_jokes?keys=true | python3 -mjson.tool
