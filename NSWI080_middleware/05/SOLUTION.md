# Task 05 solution

## How to run

I implemented the document system with hazelcast. to run it:

1. build the project:
```
mvn clean install
```

2. run the member (server):
```
mvn exec:java@member
```

3. run the client with username:
```
mvn exec:java@client -Dexec.args=username
```

## Data structures overview

i stored all the data in hazelcast maps:
- `documents` - cache for documents
- `selectedDocuments` - current document for each user
- `viewCounts` - how many times each document was viewed
- `comments` - comments for each document
- `favorites` - list of favorite documents for each user


## Configuration of data structures

### document map
the document cache can get really big so i set it up with LRU eviction. this means the least recently used documents get removed when memory is low. i also added a TTL of 1 hour so documents don't stay in memory forever if nobody uses them.

since documents can be regenerated, we dont need to backup them. this saves memory.

### comments, view counts and favorites
These maps have IMPORTANT data that can't be recreated if lost. they're probably small so i don't need eviction. i set backup-count to 3 to make sure we don't lose this data.

### selected document map
This isn't super critical - if it's lost, users just need to select their document again. I used 1 backup.

## how i avoided race conditions
For things like incrementing view counts or adding comments, i used `EntryProcessors`. this makes operations atomic avoiding race conditions


I hope all the requirements were met.