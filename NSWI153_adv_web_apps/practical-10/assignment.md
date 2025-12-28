
# Advanced Web Applications  
## Docker with Compose

---

### Get Ready

- Bring your own computer to the practical.
- Make sure you can execute the following command:
  ```sh
  docker run hello-world
  ```
- Before attending, run:
  ```sh
  docker pull php:8.3.6-apache
  ```
- You need to have the `nswi153/frontend` Docker image from the previous practical.

---

### Objectives

- Learn Docker Compose

---

### Docker Compose Commands

- **Build all images:**  
  ```sh
  docker compose build [<service>]
  ```
- **Build and start services, create volumes, bind ports, etc.:**  
  ```sh
  docker compose up [-d] [--remove-orphans] [<service>]
  ```
- **Stop services and remove containers, network:**  
  ```sh
  docker compose down [<service>]
  ```
- **Similar to `docker exec`:**  
  ```sh
  docker compose exec [<service>] [<command>]
  ```
- **Show output (stdout/stderr) for services:**  
  ```sh
  docker compose logs [-f] [<service>]
  ```

---

### Docker Compose File

- Services are defined in a file.
- Values for environment variables are automatically loaded from `.env`.
- Useful fields:
  - `restart`
  - `depends_on`
  - `healthcheck`
  - `volumes`
  - Variable defaults
  - Variables and `.env` file

#### Example `docker-compose.yaml`

```yaml
version: '3.8'
services:
  frontend:
    image: nswi153/frontend
    ports:
      - 8090:80
    environment:
      SERVER_URL: 'https://webik.ms.mff.cuni.cz/nswi153/2024-2025/service/09/api/'
```

---

## Assignment: Backend

**Location:** `./practical-10/`

1. **Copy** `practical-03/backend` and `practical-09/frontend` from previous practicals to `practical-10`.
2. **Create a Dockerfile** for a container hosting the web application backend.
   - Name the image `nswi153/backend`.
   - Update the source code so that:
     - Database host can be set using `DATABASE_HOST`.
     - Database driver can be set using `DATABASE_DRIVER`.
   - Use `php:8.3.6-apache` as a base image.
   - You are allowed to modify the copied files.

#### Steps to create a functional Docker image:

- Enable rewrite and allow its use in `.htaccess`:
  ```sh
  sed -i 's/AllowOverride None/AllowOverride All/' /etc/apache2/apache2.conf
  a2enmod rewrite
  ```
- Update package index and install requirements for Composer and pgsql:
  ```sh
  apt-get update
  apt-get -y --no-install-recommends install unzip libpq-dev
  ```
- Install PHP extensions:
  ```sh
  docker-php-ext-install pgsql
  ```
- Install Composer and use it to install all dependencies.

---

## Assignment: Docker Compose

**Location:** `./practical-10/`

- Create a `docker-compose.yaml` file hosting:
  - `nswi153/frontend` (build from `./frontend`)
  - `nswi153/backend` (build from `./backend`)
  - `database`
  - `adminer`

---

## Assignment: Docker Compose - Postgres

**Location:** `./practical-10/postgres`

- Create a Dockerfile with `postgres:16.2`.
- Automatically populate the database `nswi153` with articles and authors.
- Once ready, utilize the image in the docker-compose file as the database.
- You may need to modify other files in the `practical-10` directory to make it work.

#### Notes:

- For image `postgres:16.2`, you can copy SQL scripts to `/docker-entrypoint-initdb.d`.  
  Those scripts get automatically executed after a database is created.
- Declare a named volume for postgres data:  
  `"postgres_data:/var/lib/postgresql/data"`

---

## Submission

- You must implement and push the assignments to GitLab before **8.5.2025 23:59 UTC**.
