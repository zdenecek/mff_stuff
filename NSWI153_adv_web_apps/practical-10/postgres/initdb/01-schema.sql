CREATE SEQUENCE IF NOT EXISTS authors_id_seq;
CREATE TABLE IF NOT EXISTS "authors" (
    id INTEGER NOT NULL DEFAULT nextval('authors_id_seq') PRIMARY KEY,
    name VARCHAR(128) UNIQUE NOT NULL
);
ALTER SEQUENCE authors_id_seq OWNED BY authors.id;

CREATE SEQUENCE IF NOT EXISTS articles_id_seq;
CREATE TABLE IF NOT EXISTS "articles" (
    id INTEGER NOT NULL DEFAULT nextval('articles_id_seq') PRIMARY KEY,
    author_id INTEGER REFERENCES "authors"(id) ON DELETE SET NULL,
    title VARCHAR(128) NOT NULL,
    content VARCHAR(1024) NOT NULL
);
ALTER SEQUENCE articles_id_seq OWNED BY articles.id; 