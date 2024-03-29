import React from 'react';
import {Card} from '@material-ui/core';
import { CardContent, CircularProgress, Typography, CardHeader, Divider, Grid, Box } from '@material-ui/core';
import { urls } from '../dicts';
import Activities from './Activities';
import Strategy from './Strategy';

class Status extends React.Component {
    constructor(props){
        super(props);

        this.state = {
            status: {},
            loaded: false,
            rerender: 1,
        }
        this.handleRender = this.handleRender.bind(this);
    }

    refreshStatus(){
        fetch(urls.getStatus)
        .then(resp => resp.json())
        .then(resp => {
            this.setState({status: resp, loaded:true})
        })
        .catch((err)=> console.log(err));
    }

    componentDidMount(){
        this.refreshStatus();
        this.interval = setInterval(() => this.refreshStatus(), 1000*5); // TODO
    }
    componentWillUnmount(){
        clearInterval(this.interval);
    }

    wateringNow(){
        return(
            this.state.status.status===1 ? 
            <CardContent>
                <Typography variant="h5">Watering now</Typography>
                <br></br>

                <Typography variant="subtitle2" display="inline">remaining time: </Typography>
                <Typography variant="body2" display="inline">{this.state.status.leftTimeSec} sec</Typography>
                <br></br>
                <Typography variant="subtitle2" display="inline">working strategy: </Typography>
                <Typography variant="body2" display="inline">{this.state.status.strategy}</Typography>
            </CardContent> 
            : 
            <CardContent>
                <Typography variant="h5">Not watering now</Typography>
            </CardContent>
        );
    }

    handleRender = () => {
        console.log(this.state.rerender)
        this.refreshStatus();
    }

    render() {
        if(this.state.loaded===false){
            return(
                <CircularProgress />
            );
        }
        else{
            return (
                <>
                <Grid item sm={6} xs={12} container >
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
                </Grid>

                {/* <Grid item className={this.state.status.status===1 ? "none" : "none"} sm={3} xs={12} container >
                    <Grid item xs={12}>
                        <Card>
                            <CardHeader title="Working strategy"/>
                            <Divider variant="middle"/>
                            <CardContent>
                                <Strategy strategy={this.state.status.strategy}/>
                            </CardContent>
                        </Card>
                    </Grid>
                </Grid> */}

                </>
            );
        }
    }
  }
  
export default Status;