cd webserver
npm install
npm run build
npm start &

cd ..

go run computeserver/main.go &

go run computeclient/main.go 127.0.0.1
