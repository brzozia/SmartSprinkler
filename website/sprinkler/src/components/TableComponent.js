import React from 'react';
import Strategy from './Strategy.js'
import {urls} from '../dicts.js'
import { Typography, Box, Collapse, Icon, Paper, Switch, TableContainer, IconButton, Table, TableHead, TableBody, TableRow, TableCell } from '@material-ui/core';


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
            this.setState({strategies: response, loaded: true});
        })
        .catch(err => console.log(err));
    }

    handleSwitch(row){
      this.addRow();
      this.setState({rows: [this.getRowElement()]});
  
      let formData = new FormData();
      formData.append('enabled', (row.enabled===true ? 1 : 0));

      fetch(urls.getStrategy+row.name, {
        method: "PUT",
        body: formData
      });

      this.setState((state) => {
        let newStrategies =  state.strategies.map(el => {
          if(el===row){
            el.enabled=!el.enabled;
          }
        })
        return({strategies: newStrategies});
      })
    }

    render(){
      if(this.state.loaded===false){
        return(
            <></>
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
                    <Row key={row.name} row={row} handleSwitch={() => this.handleSwitch(row)}/>
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
              {/* {this.props.row.enabled.toString()} */}
              <Switch checked={this.props.row.enabled} onChange={this.props.handleSwitch} name="enabled" />
            </TableCell>
          </TableRow>
          <TableRow>
            <TableCell style={{ paddingBottom: 0, paddingTop: 0 }} colSpan={6}>
              <Collapse in={this.state.open} timeout="auto" unmountOnExit>
                <Box margin={1}>
                    <Strategy strategy={this.props.row.name} />
                </Box>
              </Collapse>
            </TableCell>
          </TableRow>
        </>
        );
    }
}

export default TableComponent