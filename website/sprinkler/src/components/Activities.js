import React from 'react';
import Grid from '@material-ui/core/Grid';
import Card from '@material-ui/core/Card';
import { CardContent, InputAdornment, CardActions, Icon, IconButton, TextField } from '@material-ui/core';

class WhileNotWateringActivities extends React.Component{
    render() {
          return(
            <Card>
                <Grid container direction="row" alignItems="center">
                    <Grid item sm={10}>
                        <CardContent>
                            <form noValidate autoComplete="off">
                                <TextField id="outlined-basic" InputProps={{endAdornment: <InputAdornment position="end">min</InputAdornment>,}}  label="Watering time" variant="outlined" />
                            </form>
                        </CardContent>
                    </Grid>
                    <Grid item sm={2}>
                        <CardActions>
                            <IconButton aria_label="resume watering">
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
                    
                    <IconButton aria_label="pause watering">
                        <Icon >pause</Icon>
                    </IconButton>

                    <IconButton aria_label="resume watering" onClick={() => alert('clicked')}>
                        <Icon >play_arrow</Icon>
                    </IconButton>

                    <IconButton aria_label="stop watering">
                        <Icon>stop</Icon>
                    </IconButton>
                    
                </CardActions>
            </Card>
          );
        }
  }

  class Activities extends React.Component{
      render(){
          return(
              this.props.working==="true" ? <WhileWateringActivities /> : <WhileNotWateringActivities />
          );
      }
  }


  export default Activities;