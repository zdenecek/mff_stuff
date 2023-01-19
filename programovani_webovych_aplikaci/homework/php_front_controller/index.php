<?php

/*
 * Your code goes here...
 */


function parse_template($file)
{
    if (is_dir("templates/" . $file)) {
        $page = $file . "/index.php";
    } else {
        $page = $file . ".php";
    }
    if (!file_exists("templates/" . $page)) {
        return false;
    }

    return $page;
}

function get_parameters($name)
{
    $path = "parameters/" . $name;
    if (!file_exists($path)) {
        return [];
    }

    $params = include $path;
    $keys = array_keys($params);
    $data = [];
    foreach ($keys as $key) {
        if (!key_exists($key, $_GET)) {
            error("Missing param");
        }
        $data[$key] = $_GET[$key];
        $type = $params[$key];
        if ($type === 'int') {
            if (!is_numeric($data[$key])) {
                error("Bad numeric param");
            }
            $data[$key] = (int)($data[$key]);
            continue;
        }

        if (is_array($type)) {
            if (!in_array($_GET[$key], $type)) {
                error("Bad array param");
            }
        }
    }

    return $data;
}

function render_template($___name)
{
    $params = get_parameters($___name);

    extract($params, EXTR_OVERWRITE);


    require "templates/_header.php";
    try {
        require  "templates/" . $___name;
    } catch (Exception $___e) {
        echo $___e->getMessage();
        http_response_code(500);
    }
    require "templates/_footer.php";
    die;
}


function error($message, $code = 400)
{
    echo $message;
    http_response_code($code);
    die;
}

function handle_request()
{
    if (!array_key_exists("page", $_GET)) {
        error("No page");
    }

    $template_name = $_GET['page'];

    if (preg_match("/^[a-zA-Z\/]+$/", $template_name)) {
        $path = parse_template($template_name);

        if ($path === false) {
            error("Not found", 404);
        }
    
        render_template($path);
        die;
    }

    else {
        error("Bad page");
    }

   
}

handle_request();

