# Trading app assignment with messaging

**Design** and **reasoning** about the communication **protocol** used:

The messages include everything necessary as described in the task and TODO comments, see below.

I also added a purge.sh script which purges following queues: "BankQueue" "client1SaleQueue" "client2SaleQueue" "client3SaleQueue".

## Addressed scenarios

> Considering that the buyer may transfer less money than the actual price of the required item (assuming the price is fixed). In that case, the sale should be canceled as well.

If less money or more money is transferred, the amount is returned in full and the reservation removed. The sale is then unconfirmed (cancelled)

> Consider the buyer's account balance. Refuse the sale if the buyer does not have enough money. In that case, the item should return to the seller's offer and be available for others again.

This was the more complex part. As the information about insufficient funds should be sent the the sender not the receiver (common sense), the buying protocol had to be extended.

When the sender (buyer) receives insuffient funds notice, it sends the seller a cancellation notice. 

To distinguish which transaction failed, the bank provides a way to add metadata to transaction order, returning
it in case insufficient funds in the message.

>  **Keep proper account balances.** For simplicity, newly created accounts can have a fixed balance. The balance is updated when money is transferred. If the sender's account does not have enough money to be transferred, the bank should refuse the transfer.

Balances are kept, not persisted on restart. Initial balance is 1000.

>  **Support for account balance queries.** Add a new user command to the client to show the current balance.

Added new command `a`

> **Improve the availability of the goods offered.** After a new client connects, it should immediately see offers of all other clients without any user action on those clients.

On connecting, a new client sends a Request Offers message on the Offer topic, asking all clients to publish topics.

## Scenarios not addressed

The only extensions were to address the scenarious described above.
Because of the scope of the assignment I did not address many other scenarios. 
As the task was large already and most of it was just some bussiness logic and not really helping me learn messaging, I did not go further with the implementation. However, I believe this was intended in the task as making a really robust system would be quite time consuming.

Examples of scenarios I did not handle:
- Hanging offers - client sends buy offer but does not cancel nor send transaction.
- Hanging seller
- Bank not available - basically when any party disconnects, the system becomes fragile, because of some synchronous message communication (which was part of the task)

## Message types

I generated the following docs with help of a chatbot as it is very much a manual task with no thinking (only the descriptions of all messages).

Again, the reasoning is simple, the messages include anything needed for consumers. The metadata part is described above, anything else is trivial.

### Bank Messages

#### Account Creation Request
- **Type**: TextMessage
- **From**: Client
- **To**: Bank
- **Content**: 
  - Text: `NEW_ACCOUNT_MSG`
  - Properties:
    - `CLIENT_NAME_PROPERTY`: Client's name
  - `JMSReplyTo`: Temporary queue for response

#### Account Creation Response
- **Type**: TextMessage
- **From**: Bank
- **To**: Client
- **Content**: 
  - Text: Account number as string

#### Balance Check Request
- **Type**: TextMessage
- **From**: Client
- **To**: Bank
- **Content**:
  - Text: `CHECK_BALANCE_MSG`
  - Properties:
    - `CLIENT_NAME_PROPERTY`: Client's name
    - `ACCOUNT_NUMBER_KEY`: Account number to check
  - `JMSReplyTo`: Temporary queue for response

#### Balance Check Response
- **Type**: TextMessage
- **From**: Bank
- **To**: Client
- **Content**:
  - Properties:
    - `BALANCE_KEY`: Current balance (if account exists)
  - Text: "Account does not exist" (if account doesn't exist)

#### Money Transfer Request
- **Type**: MapMessage
- **From**: Client
- **To**: Bank
- **Content**:
  - Properties:
    - `CLIENT_NAME_PROPERTY`: Sender's name
    - `ORDER_METADATA_KEY`: Seller's name (for insufficient funds handling)
  - Map entries:
    - `ORDER_TYPE_KEY`: `ORDER_TYPE_SEND`
    - `ORDER_RECEIVER_ACC_KEY`: Receiver's account number
    - `AMOUNT_KEY`: Amount to transfer

#### Transfer Report
- **Type**: MapMessage
- **From**: Bank
- **To**: Client
- **Content**:
  - Map entries:
    - `REPORT_TYPE_KEY`: 
      - `REPORT_TYPE_RECEIVED` (1): Transfer successful
      - `REPORT_TYPE_INSUFFICIENT_BALANCE` (2): Insufficient funds
    - `REPORT_SENDER_ACC_KEY`: Sender's account number
    - `AMOUNT_KEY`: Amount involved
    - `ORDER_RECEIVER_ACC_KEY`: Receiver's account number (for insufficient funds)
    - `ORDER_METADATA_KEY`: Seller's name (for insufficient funds)

### Client-to-Client Messages

#### Goods Offer
- **Type**: ObjectMessage
- **From**: Client
- **To**: All clients (via topic)
- **Content**:
  - Properties:
    - `CLIENT_NAME_PROPERTY`: Seller's name
    - `CLIENT_MESSAGE_TYPE_KEY`: `CLIENT_MESSAGE_TYPE_GOODS_LISTING`
  - Object: List of `Goods` being offered

#### Request Goods Offer
- **Message Type**: TextMessage
- **Properties**:
  - `clientName`: Name of the requesting client
  - `MessageType`: "RequestOffers"
- **Content**: "RequestOffers"
- **Purpose**: Broadcast request for current offers from all clients
- **Response**: Each client receiving this message will respond with their current goods offers using a Goods Offer message
- **Usage**: Sent by new clients when they connect to immediately receive all current offers in the system

#### Sale Request
- **Type**: MapMessage
- **From**: Buyer
- **To**: Seller
- **Content**:
  - Properties:
    - `CLIENT_NAME_PROPERTY`: Buyer's name
    - `CLIENT_MESSAGE_TYPE_KEY`: `CLIENT_MESSAGE_TYPE_OFFER`
    - `CLIENT_GOODS_NAME_KEY`: Name of requested goods
    - `CLIENT_ACCOUNT_NUMBER_KEY`: Buyer's account number
  - `JMSReplyTo`: Queue for seller's response

#### Sale Response
- **Type**: ObjectMessage
- **From**: Seller
- **To**: Buyer
- **Content**:
  - Properties:
    - `CLIENT_OFFER_ACCEPTED_KEY`: Boolean indicating if sale accepted
    - `CLIENT_PRICE_KEY`: Price of goods (if accepted)
    - `CLIENT_ACCOUNT_NUMBER_KEY`: Seller's account number (if accepted)

#### Sale Cancellation
- **Type**: MapMessage
- **From**: Buyer
- **To**: Seller
- **Content**:
  - Properties:
    - `CLIENT_NAME_PROPERTY`: Buyer's name
    - `CLIENT_MESSAGE_TYPE_KEY`: `CLIENT_MESSAGE_TYPE_OFFER_CANCELLATION`

#### Sale Confirmation
- **Type**: TextMessage
- **From**: Seller
- **To**: Buyer
- **Content**:
  - Properties:
    - `CLIENT_SALE_CONFIRMED`: Boolean indicating if sale confirmed
  - Text: Confirmation message or reason for cancellation

