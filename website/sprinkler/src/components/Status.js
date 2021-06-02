import React from 'react';
import {Card} from '@material-ui/core';
import { CardContent, Typography, CardHeader, Divider, Grid, Box } from '@material-ui/core';
import { urls } from '../dicts';
import Activities from './Activities';

class Status extends React.Component {
    constructor(props){
        super(props);

        this.state = {
            status: {},
            loaded: false,
            rerender: true,
        }
        this.handleRender = this.handleRender.bind(this);
    }

    componentDidMount(){
        fetch(urls.getStatus)
        .then(resp => resp.json())
        .then(resp => {
            this.setState({status: resp, loaded:true})
        })
    }
    
    wateringNow(){
        return(
            this.state.status.status===1 ? 
            <CardContent>
                <Typography variant="h6">Current watering</Typography>
                <br></br>

                <Typography variant="subtitle2" display="inline">remaining time: </Typography>
                <Typography variant="body2" display="inline">{this.state.status.leftTimeSec} sec</Typography>
                <br></br>
                <Typography variant="subtitle2" display="inline">working strategy: </Typography>
                <Typography variant="body2" display="inline">{this.state.status.strategy}</Typography>
            </CardContent> 
            : 
            <CardContent>
                <Typography variant="h6">Not watering now</Typography>
            </CardContent>
        );
    }

    handleRender = () => {
        window.location.reload();
    }

    render() {
        if(this.state.loaded===false){
            return(
                <></>
            );
        }
        else{
            return (
                <>
                    <Grid item xs={12}>
                        <Box m={1}>
                            <Card >
                                <CardHeader title="Status" className={this.state.status.status===1 ? "green" : "red"}/>
                                <Divider variant="middle"/>
                                <div className={this.state.status.status===1 ? "light-green" : "light-red"}>
                                    {this.wateringNow()}
                                </div>
                            </Card>
                        </Box>
                    </Grid>

                    <Grid item xs={12}>
                        <Box m={1}>
                            <Activities status={this.state.status.status} onChange={this.handleRender}/>
                        </Box>
                    </Grid>
                </>
            );
        }
    }
  }
  
export default Status;