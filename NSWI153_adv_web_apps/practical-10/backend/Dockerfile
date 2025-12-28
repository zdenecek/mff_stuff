FROM php:8.3.6-apache

# Enable Apache mod_rewrite and allow .htaccess overrides
RUN sed -i 's/AllowOverride None/AllowOverride All/' /etc/apache2/apache2.conf \
    && a2enmod rewrite

# Install system dependencies for Composer and PostgreSQL
RUN apt-get update \
    && apt-get -y --no-install-recommends install unzip libpq-dev postgresql-client \
    && rm -rf /var/lib/apt/lists/*

# Install PHP extensions
RUN docker-php-ext-install pgsql pdo pdo_pgsql

# Install Composer
COPY --from=composer:2.7 /usr/bin/composer /usr/bin/composer

# Set working directory
WORKDIR /var/www/html

# Copy application files
COPY . /var/www/html

# Install PHP dependencies
RUN composer install --no-interaction --no-dev --optimize-autoloader

# Create .env file if not exists
RUN echo "BASE_URL=" > /var/www/html/.env

# Expose port 80
EXPOSE 80

# Create entrypoint script
COPY docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh
RUN chmod +x /usr/local/bin/docker-entrypoint.sh

# Set the entrypoint to our custom script
ENTRYPOINT ["docker-entrypoint.sh"]
CMD ["apache2-foreground"] 