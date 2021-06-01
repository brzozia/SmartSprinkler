import React from 'react';
import Grid from '@material-ui/core/Grid';
import Card from '@material-ui/core/Card';
import { CardContent, InputAdornment, CardActions, Icon, IconButton, TextField } from '@material-ui/core';
import { urls } from '../dicts';

class WhileNotWateringActivities extends React.Component{
    // constructor(props){
    //     super(props);

    //     this.state = {
    //         duration: 0,
    //     }
    // }

    // handleDuration = (e) =>{
    //     this.setState({duration:e.target.value});
    // }

    render() {
          return(
            <Card>
                <Grid container direction="row" alignItems="center">
                    <Grid item sm={10}>
                        <CardContent>
                            <form noValidate autoComplete="off">
                                <TextField id="outlined-basic" InputProps={{endAdornment: <InputAdornment position="end">min</InputAdornment>,}} 
                                value={this.props.duration} 
                                onChange={this.props.handleDuration} 
                                label="Watering time" 
                                variant="outlined" />
                            </form>
                        </CardContent>
                    </Grid>
                    <Grid item sm={2}>
                        <CardActions>
                            <IconButton aria_label="resume watering" onClick={this.props.start}>
                                <Icon>play_arrow</Icon>
                            </IconButton>                    
                        </CardActions>
                    </Grid>
                </Grid>
            </Card>
          );
        }
  }

  class WhileWateringActivities extends React.Component{
    render() {
          return(
            <Card>
                <CardActions display="row" justify="space-around">
                    
                    <IconButton aria_label="pause watering" disabled>
                        <Icon >pause</Icon>
                    </IconButton>

                    <IconButton aria_label="resume watering" onClick={this.props.start} disabled>
                        <Icon >play_arrow</Icon>
                    </IconButton>

                    <IconButton aria_label="stop watering" onClick={this.props.stop}>
                        <Icon>stop</Icon>
                    </IconButton>
                    
                </CardActions>
            </Card>
          );
        }
  }

  class Activities extends React.Component{
    constructor(props){
        super(props);

        this.state = {
            duration: 0,
        }
        this.handleDuration = this.handleDuration.bind(this);
    }
    
    start = () => {
        let formData = new FormData();
        formData.append('duration', this.state.duration);

        fetch(urls.postStart, {
            method:"POST",
            body: formData
        })
        .then(this.props.onChange);
    }

    stop = () => {
        fetch(urls.postStop, {
            method:"POST"
        })
        .then(this.props.onChange);
    }

    handleDuration = (e) =>{
        this.setState({duration:e.target.value});
    }


    render(){
        const {duration} = this.state;
        return(
            this.props.status===1 ? <WhileWateringActivities stop={this.stop} start={this.start}/> : <WhileNotWateringActivities duration={duration} handleDuration={this.handleDuration} start={this.start}/>
        );
    }
  }


  export default Activities;