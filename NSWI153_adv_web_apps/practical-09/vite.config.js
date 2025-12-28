// vite.config.js
import { resolve } from 'path'
import { defineConfig, loadEnv } from 'vite'

export default defineConfig(({ command, mode }) => {
	// https://vitejs.dev/config/#using-environment-variables-in-config
	const env = loadEnv(mode, process.cwd(), '')

	return {
		base: "./",
		build: {
			rollupOptions: {
				input: {
					main: resolve(__dirname, 'index.html'),
					list: resolve(__dirname, 'list.html'),
					about: resolve(__dirname, 'about.html'),
				},
			},
		},
		server: {
			proxy: {
				'/api/': {
					target: env.SERVER_URL,
					changeOrigin: true,
				}
			}
		}
	}
});

