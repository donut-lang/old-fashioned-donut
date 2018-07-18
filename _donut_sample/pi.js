var arcTan = function(x) {
  var ret = 0.0;
  var sig = x;
  var sqx = x * x;
  for(var i = 0.0; sig != 0.0; i+=1.0) {
    ret += sig / (i + i + 1.0);
    sig = -sig * sqx;
  };
  return ret;
};

for(var i=0;i<10000 * 1000;i++){
  var pi = 16.0 * arcTan(1.0 / 5.0) - 4.0 * arcTan(1.0 / 239.0);
};

console.log(pi);