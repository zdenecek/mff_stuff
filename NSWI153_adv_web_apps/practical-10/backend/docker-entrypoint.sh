#!/bin/bash
set -e

# Wait for postgres to be ready
echo "Waiting for PostgreSQL to start..."
until PGPASSWORD=$DATABASE_PASSWORD psql -h $DATABASE_HOST -U $DATABASE_USER -d $DATABASE_NAME -c '\q'; do
  echo "PostgreSQL is unavailable - sleeping"
  sleep 1
done

echo "PostgreSQL is up - checking if schema exists"

# Check if articles table exists
TABLE_EXISTS=$(PGPASSWORD=$DATABASE_PASSWORD psql -h $DATABASE_HOST -U $DATABASE_USER -d $DATABASE_NAME -t -c "SELECT EXISTS (SELECT FROM information_schema.tables WHERE table_name = 'articles');")

if [[ $TABLE_EXISTS == *"t"* ]]; then
  echo "Tables already exist - skipping schema creation"
else
  echo "Creating schema with Doctrine"
  php /var/www/html/create-schema.php

  echo "Setting up sample data"
  PGPASSWORD=$DATABASE_PASSWORD psql -h $DATABASE_HOST -U $DATABASE_USER -d $DATABASE_NAME -c "
  INSERT INTO authors (name) VALUES ('Alice'), ('Bob'), ('Charlie') ON CONFLICT (name) DO NOTHING;
  " || true

  PGPASSWORD=$DATABASE_PASSWORD psql -h $DATABASE_HOST -U $DATABASE_USER -d $DATABASE_NAME -c "
  INSERT INTO articles (author_id, title, content) VALUES 
      (1, 'First Article', 'This is the content of the first article.'),
      (2, 'Second Article', 'This is the content of the second article.'),
      (3, 'Third Article', 'This is the content of the third article.');
  " || true
fi

echo "Setup complete - starting Apache"

# Execute the main command
exec "$@" 