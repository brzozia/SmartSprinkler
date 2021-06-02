let connectorTypes = {
    1: "and",
    2: "or"
}

let T = {
    1: "condition check",
    2: "connector types",
    3: "result"
}

let D = {
    1: "air humidity",
    2: "soil humidity",
    3: "air temperature",
    4: "wind speed",
    5: "time"    
}
 
let C = {
    1: "<",
    2: "<=",
    3: ">",
    4: ">=",
    5: "="
}

let prefix = "";

let urls = {
    getStrategy: prefix+"strategy/",
    getStrategies: prefix+"strategies",
    getSensors: prefix+"sensors",
    getStatus: prefix+"status",
    postStop: prefix+"stop",
    postStart: prefix+"start",
}

export {T,C,D, connectorTypes, urls}