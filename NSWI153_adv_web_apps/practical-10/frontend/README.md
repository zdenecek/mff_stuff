# Tortura

## Building the Docker Image

```bash
docker build -t nswi153/frontend .
```

## Running the Docker Container

```bash
docker run -p 8080:80 nswi153/frontend
```

## Configuration

The application is configured to use an API proxy. By default, the API URL is set to `https://webik.ms.mff.cuni.cz/nswi153/2024-2025/service/09/api/`. 

You can override this URL by providing a custom `SERVER_URL` environment variable:

```bash
docker run -p 8080:80 -e SERVER_URL="https://your-custom-api-url.com/api/" nswi153/frontend
```
