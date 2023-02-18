import React from "react";
import logoSEL from "./SEL_Logo_White.svg";
import headShot from "./james_acaster.png";
import "./AppBar.css";

function AppBar() {
  return (
    <div className="app-bar">
      <img
        className="logo"
        src={logoSEL}
        alt="the letters SEL bounded by a rounded rectangle"
      />
      <h2 className="app-name">Test Station Tool</h2>

      <svg role="img" viewBox="0 0 24 24" title="get help">
        <path d="M12,4C7.58,4,4,7.58,4,12s3.58,8,8,8,8-3.58,8-8S16.42,4,12,4Zm-1,3h2v2h-2v-2Zm3,10h-4v-1h1v-4h-1v-1h3v5h1v1Z"></path>
      </svg>
      <img className="avatar" src={headShot} alt="James Acaster" />
    </div>
  );
}

export default AppBar;
