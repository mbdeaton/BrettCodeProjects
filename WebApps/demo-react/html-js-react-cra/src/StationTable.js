import React from "react";
import { station } from "./Utils";
import "./StationTable.css";

function StationTable() {
  const labelRef = React.useRef();
  const hostnameRef = React.useRef();
  const [stationList, setStationList] = React.useState([
    station("EA_STA_100", "MFGPC0"),
    station("EA_STA_101", "MFGPC1"),
    station("EA_STA_102", "MFGPC2"),
  ]);

  function handleSubmit(event) {
    event.preventDefault();
    setStationList([
      ...stationList,
      station(labelRef.current.value, hostnameRef.current.value),
    ]);
    event.target.reset();
  }

  return (
    <>
      <form id="new-row-form" onSubmit={handleSubmit}></form>
      <table>
        <thead>
          <tr>
            <th>Station Label</th>
            <th>PC Hostname</th>
          </tr>
        </thead>
        <tbody>
          {stationList.map((x) => (
            <tr key={x.label}>
              <td>{x.label}</td>
              <td>{x.hostname}</td>
            </tr>
          ))}
        </tbody>
        <tfoot>
          <tr>
            <td>
              <input
                id="stationLabel"
                type="text"
                placeholder="label"
                form="new-row-form"
                ref={labelRef}
              />
            </td>
            <td>
              <input
                id="pcHostname"
                type="text"
                placeholder="hostname"
                form="new-row-form"
                ref={hostnameRef}
              />
            </td>
            <td>
              <button className="submit-btn" type="submit" form="new-row-form">
                +
              </button>
            </td>
          </tr>
        </tfoot>
      </table>
    </>
  );
}

export default StationTable;
