import React from 'react';
import { FormControl, InputLabel, Select, Icon, Card, IconButton, FormControlLabel,TextField, Button, Grid, Switch, Box, CardHeader, CardContent} from '@material-ui/core';
import { T, D, C, connectorTypes, urls } from '../dicts';


class StrategyRow extends React.Component{
    constructor(props){
        super(props);

        this.state = {
            T: "1",
            D: "",
            C: "",
            V: 0,
        }
    }

    handleTChange = (e) =>{
        this.setState(() => {return ({T: e.target.value})});
        this.props.handleTChange(e.target.value);
    }
    handleDChange = (e) =>{
        this.setState({D: e.target.value});
        this.props.handleDChange(e.target.value);
    }
    handleCChange = (e) =>{
        this.setState({C: e.target.value});
        this.props.handleCChange(e.target.value);
    }
    handleVChange = (e) =>{
        this.setState({V: e.target.value});
        this.props.handleVChange(e.target.value);
    }

    render(){
        return(
            <Grid item container xs={12} spacing={1}>
                <Grid item xs={3}>
                    <FormControl variant="outlined">
                        <InputLabel>Type</InputLabel>
                        <Select
                        native
                        value={this.state.T}
                        onChange={this.handleTChange}
                        label="Type"
                        >
                        {Object.keys(T).map((item, i) => {
                            return (
                            <option key={i} value={item}> {T[item.toString()]} </option>
                            );
                        })}
                        </Select>
                    </FormControl>
                </Grid>
                    
                <Grid item  xs={3} className={this.state.T==="1" ? "" : "none"}>
                    <FormControl variant="outlined">
                        <InputLabel>Data</InputLabel>
                        <Select
                        native
                        value={this.state.D}
                        onChange={this.handleDChange}
                        label="Data"
                        >
                        {Object.keys(D).map((item, i) => {
                            return (
                            <option key={i} value={item}> {D[item.toString()]} </option>
                            );
                        })}
                        </Select>
                    </FormControl>
                </Grid>               
                
                <Grid item  xs={2} className={this.state.T==="1" ? "" : "none"}>
                    <FormControl variant="outlined">
                        <InputLabel>Condition</InputLabel>
                        <Select
                        native
                        value={this.state.C}
                        onChange={this.handleCChange}
                        label="Condition"
                        >
                        {Object.keys(C).map((item, i) => {
                            return (
                            <option key={i} value={item}> {C[item.toString()]} </option>
                            );
                        })}
                        </Select>
                    </FormControl>
                </Grid>

                <Grid item xs={2} className={this.state.T==="2" ? "none" : ""}>
                    <FormControl variant="outlined"> 
                        <form noValidate autoComplete="off">
                            <TextField  
                            value={this.state.V} 
                            onChange={this.handleVChange} 
                            label="Value" 
                            variant="outlined" />
                        </form>
                  </FormControl>
                </Grid>
                
                <Grid item  xs={2} className={this.state.T==="2" ? "" : "none"}>
                    <FormControl variant="outlined">
                        <InputLabel>Connection</InputLabel>
                        <Select
                        native
                        value={this.state.C}
                        onChange={this.handleConnectionChange}
                        label="Connection"
                        >
                        {Object.keys(connectorTypes).map((item, i) => {
                            return (
                            <option key={i} value={item}> {connectorTypes[item.toString()]} </option>
                            );
                        })}
                        </Select>
                    </FormControl>
                </Grid>
            </Grid>);
    }
}

class AddStrategy extends React.Component {
    constructor(props){
        super(props);

        this.state = {
            strategy: [],
            rows: [this.getRowElement()],
            name: "",
            interval: 5,
            enabled: true,
            T: "1",
            D: "1",
            C: "1",
            V: "0",
            show: false,
        }
        this.handleTChange = this.handleTChange.bind(this);
        this.handleDChange = this.handleDChange.bind(this);
        this.handleCChange = this.handleCChange.bind(this);
        this.handleVChange = this.handleVChange.bind(this);
    }

    handleTChange = (e) =>{
        this.setState(() => {return ({T: e})}, console.log(this.state.T));
    }
    handleDChange = (e) =>{
        this.setState({D: e});
    }
    handleCChange = (e) =>{
        this.setState({C: e});
    }
    handleVChange = (e) =>{
        this.setState({V: e});
    }
    handleName = (e) =>{
        this.setState({name: e.target.value});
    }
    handleInterval = (e) =>{
        this.setState({interval: e.target.value});
    }
    handleSwitch = () =>{
        this.setState((state) => {return ({enabled: !state.enabled})});
    }
    handleShow = () =>{
        this.setState((state) => {return ({show: !state.show})});
    }

    parseInput(){
        let obj = {};
        obj.T = Number(this.state.T);
        switch (this.state.T){
            case "1":
                obj.D = Number(this.state.D);
                obj.C = Number(this.state.C);
                obj.V = Number(this.state.V);
                break;
            case "2":
                obj.C = Number(this.state.C);
                break;
            case "3":
                obj.V = Number(this.state.V);
                break;
        }
        return obj;
    }

    getRowElement(){
        return(<StrategyRow handleCChange={this.handleCChange} 
            handleDChange={this.handleDChange} 
            handleTChange={this.handleTChange} 
            handleVChange={this.handleVChange}
        />);
    }

    addRow(){
        let obj = this.parseInput();
        this.setState((state) => {
            return({
                strategy: [...state.strategy, obj],
                T: "1",
                D: "1",
                C: "1",
                V: "0",
                rows: [...state.rows, this.getRowElement() ]
            })
        });
    }

    cleanState(){
        this.setState(() => {
            return({
                strategy: [],
                T: "1",
                D: "1",
                C: "1",
                V: "0",
                name: "",
                interval: 5,
                rows: [this.getRowElement() ]
            })
        });
    }

    submit(){
        let obj = this.parseInput();
        let strategyEl = [...this.state.strategy,obj]
        console.log(strategyEl)
        // let formData = new FormData();
        // formData.append('name', this.state.name.replace(" ",""));
        // formData.append('enabled', this.state.enabled===true ? 1 : 0);
        // formData.append('interval', this.state.interval);
        // formData.append('body', this.state.strategy);

        fetch(urls.getStrategies, {
            method: "POST",
            headers: {
              'Content-Type':'application/x-www-form-urlencoded'
            },
            body: `name=${this.state.name.replace(" ","-")}&enabled=${this.state.enabled===true ? 1 : 0}&interval=${this.state.interval}&body=${encodeURIComponent(JSON.stringify(strategyEl))}`
        })
        .catch(err => console.log(err));

        // fetch(urls.getStrategies, {
        //     method:"POST", 
        //     body: formData
        // }).catch(err => console.log(err));

        this.cleanState();
    }

    
    render(){
        let i=0;
        return(
            <>
            <Card>
                <Grid container>
                <Grid item xs={10}>
                    <CardHeader title="Add strategy" />
                </Grid>
                <Grid item xs={2} style={{"paddingTop":"1rem"}}>
                    <Button variant="outlined" onClick={this.handleShow}>show</Button>
                </Grid>
                </Grid>
                <CardContent className={this.state.show===false ? "none" : ""}>
                    <Grid container spacing={1} style={{"marginBottom":"1rem"}}>
                        <Grid item xs={3}>
                                <FormControl variant="outlined"> 
                                    <form noValidate autoComplete="off">
                                        <TextField  
                                        value={this.state.name} 
                                        onChange={this.handleName} 
                                        label="Name" 
                                        variant="outlined" />
                                    </form>
                            </FormControl>
                        </Grid>
                        <Grid item xs={2} >
                            <FormControl variant="outlined"> 
                                <form noValidate autoComplete="off">
                                    <TextField  
                                    value={this.state.interval} 
                                    onChange={this.handleInterval} 
                                    label="Interval [min]" 
                                    variant="outlined" />
                                </form>
                        </FormControl>
                        </Grid>
                        <Grid item xs={1}>
                        <FormControlLabel
                            control={<Switch checked={this.state.enabled} onChange={this.handleSwitch} name="enabled" />}
                            label="Enabled"
                        />
                        </Grid>
                    </Grid>

                    <Grid container spacing={1} >

                        {this.state.rows.map(row => {
                            i=i+1;
                            return (
                               <React.Fragment key={i}>{row}</React.Fragment>
                            );
                        })}                        

                        <Grid item xs={2}>
                            <IconButton aria_label="add watering" onClick={() => this.addRow()}>
                                <Icon fontSize="large">add_circle</Icon>
                            </IconButton>
                        </Grid>
                    </Grid>
                    <Grid container>
                        <Grid item>
                        <Box pt={2}>
                        <Button variant="contained" onClick={() => this.submit()}>Submit</Button>
                        </Box>
                        </Grid>

                        <Grid item>
                        <Box pt={2} pl={2}>
                        <Button variant="contained" onClick={() => this.cleanState()}>Cancel</Button>
                        </Box>
                        </Grid>
                    </Grid>
                </CardContent>
            </Card>
            
        </>
        );
    }
}


// TODO this.addRow vs () => this.addRow
export default AddStrategy;