<?php ob_start(); ?>

<div class="jumbotron">
    <h1 class="display-4">Welcome to CMS Application</h1>
    <p class="lead">This is a simple content management system that allows you to manage authors and articles.</p>
    <hr class="my-4">
    <p>Use the navigation menu above to:</p>
    <ul>
        <li>View and register authors</li>
        <li>View and create articles</li>
        <li>Access the API endpoints</li>
    </ul>
</div>

<?php $content = ob_get_clean(); ?>
<?php require __DIR__ . '/layout.php'; ?> 