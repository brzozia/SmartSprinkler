import React from 'react';
import Strategy from './Strategy.js'
import { Typography, Box, Collapse, Icon, Paper, TableContainer, IconButton, Table, TableHead, TableBody, TableRow, TableCell } from '@material-ui/core';


class TableComponent extends React.Component{
    constructor(props){
        super(props);

        this.state = {
            strategies: [{name:"ale", interval:5, enabled:true}]
        }
    }
    
    componentDidMount(){
        fetch('strategies')
        .then(response => response.json())
        .then(response => {

            this.setState({strategies: response});
        })

    }

    render(){
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
                  <TableCell align="center">Interval</TableCell>
                  <TableCell align="center">Is working?</TableCell>
                </TableRow>
              </TableHead>
              <TableBody>
                {this.state.strategies.map((row) => (
                  <Row key={row.name} row={row} />
                ))}
              </TableBody>
            </Table>
          </TableContainer>
        );
    }
}

class Row extends React.Component{
  constructor(props){
      super(props);

      this.state = { 
          open: false,
          strategy: []
        }
  }

  setOpen(){
      fetch('url')
      .then(response => response.json())
      .then(response => {
        this.setState((state)=>({
            open: !state.open,
            strategy: response,
        }))
      })
      
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
            <TableCell align="center">{this.props.row.enabled.toString()}</TableCell>
          </TableRow>
          <TableRow>
            <TableCell style={{ paddingBottom: 0, paddingTop: 0 }} colSpan={6}>
              <Collapse in={this.state.open} timeout="auto" unmountOnExit>
                <Box margin={1}>
                    <Strategy strategy={this.state.strategy} />
                </Box>
              </Collapse>
            </TableCell>
          </TableRow>
        </>
        );
    }
}

export default TableComponent