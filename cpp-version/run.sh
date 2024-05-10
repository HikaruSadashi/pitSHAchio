# make sure you run npm run build in webserver/ first
cd webserver
npm run start & disown
cd ../computeserver
./ComputeServer 