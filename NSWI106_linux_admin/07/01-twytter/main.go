package main

import (
	"context"
	"database/sql"
	"fmt"
	"strconv"
	"net/http"
	"os"

	"github.com/Showmax/env"
	"github.com/dcepelik/twytter/sqlc"
	"github.com/labstack/echo/v4"

	_ "github.com/lib/pq"
)

type config struct {
	Port int16 `env:"PORT"`
	DB   struct {
		Host     string `env:"HOST"`
		Port     int16  `env:"PORT"`
		User     string `env:"USER"`
		Password string `env:"PASSWORD"`
	} `env:"DB_"`
}

type twytterError struct {
	Message string
}

func run() error {
	var cfg config
	if err := env.Load(&cfg, "TWYTTER_"); err != nil {
		return err
	}

	dsn := fmt.Sprintf("host=%s port=%d user=%s password=%s sslmode=disable",
		cfg.DB.Host,
		cfg.DB.Port,
		cfg.DB.User,
		cfg.DB.Password,
	)
	db, err := sql.Open("postgres", dsn)
	if err != nil {
		return fmt.Errorf("sql.Open: %w", err)
	}
	queries := sqlc.New(db)

	e := echo.New()
	e.Debug = false
	e.GET("/twytts", func(c echo.Context) error {
		twytts, err := queries.GetTwytts(context.TODO())
		if err != nil && err != sql.ErrNoRows {
			fmt.Println(err)
			return err
		}
		return c.JSON(http.StatusOK, twytts)
	})
	e.POST("/twytts", func(c echo.Context) error {
		var t sqlc.Twytt
		if err := c.Bind(&t); err != nil {
			return err
		}
		if t.Name == "" || t.Twytt == "" {
			return c.JSON(400, twytterError{
				Message: "both Name and Twytt must be provided",
			})
		}
		nt, err := queries.InsertTwytt(context.TODO(), sqlc.InsertTwyttParams{
			Name:  t.Name,
			Twytt: t.Twytt,
		})
		if err != nil {
			return err
		}
		return c.JSON(http.StatusOK, nt)
	})
	e.PUT("/twytts/:id", func(c echo.Context) error {
		var t sqlc.Twytt
		if err := c.Bind(&t); err != nil {
			return err
		}
		
		if t.Name == "" || t.Twytt == "" {
			return c.JSON(400, twytterError{
				Message: "Id, Name and Twytt must be provided",
			})
		}
		id, _ := strconv.ParseInt(c.Param("id"), 10, 64)
		nt, err := queries.UpdateTwytt(context.TODO(), sqlc.UpdateTwyttParams{
			ID: id,
			Name:  t.Name,
			Twytt: t.Twytt,
		})
		if err != nil {
			return err
		}
		return c.JSON(http.StatusOK, nt)
	})
	e.DELETE("/twytts/:id", func(c echo.Context) error {
		id, _ := strconv.ParseInt(c.Param("id"), 10, 64)
		nt, err := queries.DeleteTwytt(context.TODO(), id)
		if err != nil {
			return err
		}
		return c.JSON(http.StatusOK, nt)
	})
	return e.Start(fmt.Sprintf(":%d", cfg.Port))
}

func main() {
	if err := run(); err != nil {
		fmt.Fprintf(os.Stderr, "twytter: %s\n", err.Error())
		os.Exit(1)
	}
}
