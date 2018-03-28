#version 330
in vec4 theColour;
in float diffTerm;
in float ndotv;

void main()
{
   //gl_FragColor = vec4(1.0, 0.5, 0.25, 1.0);
   //gl_FragColor = theColour;
   //gl_FragColor = vec4(gl_FragCoord.z);
   if(diffTerm < 0.1){
	   gl_FragColor = vec4(0.5,0.5,0.5,1);
   }else{
	   gl_FragColor = theColour;
   }
   
   if((ndotv < 0.2) && (ndotv > -0.2)){
	   gl_FragColor = vec4(1,1,1,1);
  }	      
}
