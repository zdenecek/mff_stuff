# Build stage
FROM node:21.7.2-slim as build

# Set working directory
WORKDIR /app

# Set environment variable for the app
ARG VITE_INSTANCE_NAME=Docker
ENV VITE_INSTANCE_NAME=${VITE_INSTANCE_NAME}

# Copy package files and install dependencies
COPY package*.json ./
RUN npm install

# Copy the rest of the application code
COPY . .

# Build the application
RUN npm run build

# Hosting stage
FROM nginx:1.25.4

# Copy the built files from the build stage
COPY --from=build /app/dist /usr/share/nginx/html

# Set up the environment variable for API proxy
ENV SERVER_URL=https://webik.ms.mff.cuni.cz/nswi153/2024-2025/service/09/api/

# Copy Nginx configuration template
COPY nginx/default.conf.template /etc/nginx/templates/

# Expose port 80
EXPOSE 80 