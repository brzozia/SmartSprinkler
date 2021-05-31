import React from 'react';
import Card from '@material-ui/core/Card';
import { CardContent, Typography, CardHeader, Divider } from '@material-ui/core';

class Status extends React.Component {
    wateringNow(){
        return(
            this.props.working==='true' ? 
            <CardContent>
                <Typography variant="h6">Current watering</Typography>
                <Typography variant="subtitle2" display="inline">start: </Typography>
                <Typography variant="body2"display="inline" >{this.props.start}</Typography>
                <br></br>

                <Typography variant="subtitle2" display="inline">end: </Typography>
                <Typography variant="body2" display="inline">{this.props.end}</Typography>
                <br></br>
                <Typography variant="subtitle2" display="inline">working strategy: </Typography>
                <Typography variant="body2" display="inline">{this.props.strategyName}</Typography>
            </CardContent> 
            : 
            <>
            </>
        );
    }

    render() {
      return (
            <Card >
                <CardHeader title="Status" className={this.props.working==='true' ? "green" : "red"}/>
                <Divider variant="middle"/>
                <div className={this.props.working==='true' ? "light-green" : "light-red"}>
                {this.wateringNow()}
                <CardContent title="Status" >
                    <Typography variant="h6">Last watering</Typography>
                    <Typography variant="subtitle2" display="inline">start: </Typography>
                    <Typography variant="body2" display="inline">{this.props.lastStart}</Typography>
                    <br></br>
                    <Typography variant="subtitle2" display="inline">end: </Typography>
                    <Typography variant="body2" display="inline">{this.props.lastEnd}</Typography>
                </CardContent>
                </div>
            </Card>
      );
    }
  }
  
export default Status;