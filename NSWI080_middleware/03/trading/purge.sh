#!/bin/bash


for queue in "BankQueue" "client1SaleQueue" "client2SaleQueue" "client3SaleQueue"
do
  artemis queue purge --user admin --password admin --name "$queue" --url tcp://localhost:61616
done