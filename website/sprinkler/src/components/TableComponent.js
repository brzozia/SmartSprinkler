import React from 'react';
import Strategy from './Strategy.js'
import {urls} from '../dicts.js'
import { Typography, Box, Collapse, CircularProgress, Grid, Icon, Paper, Switch, TableContainer, IconButton, Table, TableHead, TableBody, TableRow, TableCell } from '@material-ui/core';


class TableComponent extends React.Component{
    constructor(props){
        super(props);

        this.state = {
            strategies: [],
            loaded: false
        }
        this.handleSwitch = this.handleSwitch.bind(this);
      }
    
    componentDidMount(){
        fetch(urls.getStrategies)
        .then(response => response.json())
        .then(response => {
            this.setState({strategies: (response===null ? [] : response), loaded: true});
        })
        .catch(err => console.log(err));
    }

    handleDelete(row){
      fetch(urls.getStrategy+row.name, {
        method: "DELETE",
        headers: {
          'Content-Type':'application/x-www-form-urlencoded'
        }
      });

      this.setState((state) => {
        let newStrategies = state.strategies.filter(el => {
         return el!==row;
        });
        return({strategies: newStrategies});
      })
    }

    handleSwitch(row){  
      //let formData = new FormData();
      //formData.append('enabled', (row.enabled===true ? 1 : 0));

      fetch(urls.getStrategy+row.name, {
        method: "PATCH",
        headers: {
          'Content-Type':'application/x-www-form-urlencoded'
        },
        body: `enabled=${(row.enabled===true ? 0 : 1)}`
      });

      this.setState((state) => {
        let newStrategies = state.strategies;
        newStrategies.map(el => {
          if(el===row){
            el.enabled=!el.enabled;
          }
          return el;
        })
        return({strategies: newStrategies});
      })
    }

    render(){
      if(this.state.loaded===false){
        return(
            <CircularProgress />
        );
      }
      else{
        return (
            <TableContainer component={Paper}>
                <Box pt={2} pl={3}>
                <Typography variant="h5">Strategies</Typography>
                </Box>
              <Table aria-label="collapsible table">
                <TableHead>
                  <TableRow>
                    <TableCell />
                    <TableCell>Name</TableCell>
                    <TableCell align="center">Interval [min]</TableCell>
                    <TableCell align="center">Enabled?</TableCell>
                  </TableRow>
                </TableHead>
                <TableBody>
                  {this.state.strategies.map((row) => (
                    <Row key={row.name} row={row} handleDelete={() => this.handleDelete(row)} handleSwitch={() => this.handleSwitch(row)}/>
                  ))}
                </TableBody>
              </Table>
            </TableContainer>
          );
        }
      }
}

class Row extends React.Component{
  constructor(props){
      super(props);

      this.state = { 
          open: false,
        }
  }

  setOpen(){
        this.setState((state)=>({
            open: !state.open,
        }));
  }

    render(){
        return(
          <>
          <TableRow >
            <TableCell>
              <IconButton aria-label="expand row" size="small" onClick={() => this.setOpen()}>
                {this.state.open ? <Icon>keyboard_arrow_up</Icon> : <Icon>keyboard_arrow_down</Icon>}
              </IconButton>
            </TableCell>
            <TableCell component="th" scope="row">
              {this.props.row.name}
            </TableCell>
            <TableCell align="center">{this.props.row.interval}</TableCell>
            <TableCell align="center">
              <Switch checked={this.props.row.enabled} onChange={this.props.handleSwitch} name="enabled" />
            </TableCell>
          </TableRow>
          <TableRow>
            <TableCell style={{ paddingBottom: 0, paddingTop: 0 }} colSpan={6}>
              <Collapse in={this.state.open} timeout="auto" unmountOnExit>
                <Grid container spacing={1}>
                  <Grid item style={{"padding":"0.8rem"}} xs={8}>
                    <Strategy strategy={this.props.row.name} />
                    </Grid>
                    <Grid item xs={4}>
                    <IconButton aria_label="delete strategy" onClick={this.props.handleDelete}>
                      <Icon>delete</Icon>
                  </IconButton>
                  </Grid>
                </Grid>
              </Collapse>
            </TableCell>
          </TableRow>
        </>
        );
    }
}

export default TableComponent