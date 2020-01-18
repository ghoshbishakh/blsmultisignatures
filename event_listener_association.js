var Web3 = require('web3')
let websocket_connection = new Web3(
  new Web3.providers.WebsocketProvider("wss://ropsten.infura.io/ws/v3/ac3295a643524b6fb80800fb5e9dfa1d")
);
const verifySig = require('./index.js');
const subscribedEvents = {}

const subscribeLogEvent = (contract, eventName) => {
    const eventJsonInterface = websocket_connection.utils._.find(
        contract._jsonInterface,
        o => o.name === eventName && o.type === 'event',
    )

    const subscription = websocket_connection.eth.subscribe('logs', {
        address: contract.options.address,
        topics: [eventJsonInterface.signature]
    }, (error, result) => {
        if (!error) {
            const eventObj = websocket_connection.eth.abi.decodeLog(
                eventJsonInterface.inputs,
                result.data,
                result.topics.slice(1)
            )
            console.log(`New ${eventName}!`, eventObj)
            verifySig(eventObj.encoded_signs);
            // console.log("Core requirement: " , eventObj.core_requirement);
        }
    })

    subscribedEvents[eventName] = subscription
    console.log(`subscribed to event '${eventName}' of contract '${contract.options.address}' `)
}

const unsubscribeEvent = (eventName) => {
    subscribedEvents[eventName].unsubscribe(function (error, success) {
        if (success)
            console.log('Successfully unsubscribed!');
    });
}