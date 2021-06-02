import React from 'react';
import {Grid} from '@material-ui/core';
import {Card} from '@material-ui/core';
import { CardContent,CircularProgress, Typography} from '@material-ui/core';
import { urls } from '../dicts';

class Sensor extends React.Component {
    render() {
      return (
        <Card>
            <CardContent>
                <Grid container justify="space-between">
                    <Grid item>
                     <Typography variant="subtitle1" align="left">{this.props.sensor.name} </Typography>
                    </Grid>
                    <Grid item>
                        <Typography variant="h5" align="right" display="inline">{this.props.sensor.value} </Typography>
                        <Typography variant="h5" align="right"  display="inline">{this.props.sensor.unit}</Typography>
                    </Grid>
                </Grid>
            </CardContent>
        </Card>
      );
    }
  }


  class Sensors extends React.Component {
    constructor(props){
      super(props);

      this.state = {
          sensors: {},
          loaded: false
      }
  }

  refreshSensors(){
    fetch(urls.getSensors)
    .then(resp => resp.json())
    .then(resp => {
      this.setState({sensors:resp, loaded:true});
    })
    .catch((err)=> console.log(err));
  }

  componentDidMount(){
    this.refreshSensors();
    this.interval = setInterval(() => this.refreshSensors(), 1000*60*2); // TODO
  }

  componentWillUnmount(){
      clearInterval(this.interval);
  }

    render() {
      if(this.state.loaded===false){
        return(
          <CircularProgress />
        );
    }
    else{
      return (
        this.state.sensors.map(sensor => {
          return (
            <Grid key={sensor.name} item xs={6} style={{"padding":"0 0.6rem"}}>
              <Sensor sensor={sensor}/>
          </Grid>
          );
        })

      );
    }
  }
  }

export default Sensors;