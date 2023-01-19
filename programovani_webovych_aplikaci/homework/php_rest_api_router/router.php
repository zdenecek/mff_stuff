<?php

class Router
{
    private function fail($code)
    {
        http_response_code($code);
        die;
    }

    private function getMethod()
    {
        if ($_SERVER['REQUEST_METHOD'] !== "GET" && $_SERVER['REQUEST_METHOD'] !== "POST") {
            $this->fail(405);
        }
        return strtolower($_SERVER['REQUEST_METHOD']);
    }

    private function getAction()
    {
        if (!array_key_exists("action", $_GET)) {
            $this->fail(400);
        }
        return $_GET['action'];
    }

    private function splitAction($action)
    {
        $parts = explode("/", $action);

        if ($this->validatePath($parts)) {
            return $parts;
        }

        $this->fail(400);
    }

    private function getControllerPath($name, $parts)
    {
        $path = "";
        $count = sizeof($parts) - 2;
        for ($i = 0; $i < $count; $i++) {
            $path .= $parts[$i];
            $path .= "/";
        }
        $path = "controllers/" . $path . $name . ".php";

        if (!file_exists($path)) {
            $this->fail(404);
        }

        return $path;
    }

    private function validatePath($parts)
    {
        if (sizeOf($parts) < 2) {
            return false;
        }
        foreach ($parts as $part) {
            if (!preg_match("/^[A-Za-z_]+$/", $part)) {
                return false;
            }
        }
        return true;
    }

    public function dispatch()
    {
        $method = $this->getMethod();
        $action = $this->getAction();


        $parts = $this->splitAction($action);
        $controllerName = $parts[sizeOf($parts) - 2];
        $path = $this->getControllerPath($controllerName, $parts);

        $fullControllerName = $controllerName . "Controller";
        $methodName = $method . end($parts);

        $this->invoke($path, $fullControllerName, $methodName);
    }

    public function invoke($path, $controllerName, $methodName)
    {
        require_once __DIR__ . "/" . $path;

        if (!class_exists($controllerName)) {
            $this->fail(404);
        }

        $controllerInstance = new $controllerName();

        if (!method_exists($controllerName, $methodName)) {
            $this->fail(404);
        }


        $method = new ReflectionMethod($controllerInstance, $methodName);
        $args = [];
        foreach ($method->getParameters() as $param) {
            if (key_exists($param->name, $_REQUEST)) {
                $args[$param->name] = $_REQUEST[$param->name];
            } else {
                $this->fail(400);
            }
        }

        try {
            $response = call_user_func_array([$controllerInstance, $methodName], $args);
        } catch (Exception $e) {
            $this->fail(500);
        }

				if ($response === null) {
						http_response_code(204);
						die;
				}
				echo json_encode($response);
    }
}
