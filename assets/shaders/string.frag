void main(){
    vec2 coord = gl_FragCoord.xy;

    float s = sin(coord.y) + coord.y;

    gl_FragColor = vec4(s, s, s, 1.0);
}