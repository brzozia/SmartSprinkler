import { Typography, CircularProgress } from '@material-ui/core';
import React from 'react';
import { C, connectorTypes, D, urls } from '../dicts';

    class Strategy extends React.Component{
        constructor(props){
            super(props);

            this.state ={
                strategy: [],
                loaded: false,
            }
        }

        componentDidMount(){
            fetch(urls.getStrategy+this.props.strategy)
            .then(response => response.json())
            .then(response => {
                this.setState({strategy: response, loaded:true})
            })
            .catch(err => console.log(err));
        }
        
        onCondition(line){
            return (
                <Typography variant="body1">
                    {D[line.D] + " " + C[line.C] + " "+ line.V}
                </Typography>);
        }

        onConnector(val){
            return(<Typography variant="body1">{connectorTypes[val]}</Typography>);
        }

        onResult(val){
            return(<>
            <Typography variant="body1">then</Typography>
            <Typography variant="body1">watering time {val} sec</Typography>
            </>);
        }
        
        render(){
            let i=0;
            if(this.state.loaded===false){
                return(
                  <CircularProgress />
                );
            }
            else{
            return(
            <>
                {this.state.strategy.map(line => {
                    let ret;
                    i=i+1;
                    switch (line.T){
                        case 1:
                            ret = this.onCondition(line);
                            break;
                        case 2:
                            ret = this.onConnector(line.V);
                            break;
                        case 3:
                            ret = this.onResult(line.V);
                            break;
                        default:
                            ret = (<></>);
                    }
                    return (
                        <React.Fragment key={i}>
                        {ret}
                        </React.Fragment>);
                })}
            </>
            );
            }
        }

}

export default Strategy;