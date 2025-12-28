<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>NSWI153 Assignment</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
            min-height: 100vh;
            display: flex;
            flex-direction: column;
        }
        nav {
            background-color: #333;
            padding: 1rem;
        }
        nav a {
            color: white;
            text-decoration: none;
            margin-right: 1rem;
        }
        nav a:hover {
            text-decoration: underline;
        }
        main {
            flex: 1;
            padding: 2rem;
        }
        footer {
            background-color: #333;
            color: white;
            text-align: center;
            padding: 1rem;
        }
    </style>
</head>
<body>
    <nav>
        <a href="<?= url('/') ?>">Home</a>
        <a href="<?= url('authors') ?>">Authors</a>
        <a href="<?= url('register-author') ?>">Register Author</a>
        <a href="<?= url('articles') ?>">Articles</a>
        <a href="<?= url('create-article') ?>">Create Article</a>
    </nav>
    
    <main>
        <?php echo $content; ?>
    </main>

    <footer>
        © 2024-2025 NSWI153
    </footer>
</body>
</html> 