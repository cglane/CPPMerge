var merge = require("./cpp/build/Release/merge");

var term1 = {
  key: 'americas',
  val: 5,
  translations:[
    {
      lang: 'english',
      num: 2
    },{
    lang:'portugese',
    num:25
  },{
    lang:'spanish',
    num:99
  }
  ]
};
var term2 = {
  key: 'americasss',
  val: 2,
  translations:[
    {
      lang: 'english',
      num: 5
    },
    {
      lang:'spanish',
      num:4
    }
  ]
}
var termArr = [
  {
    key: 'me',
    val: 6,
    translations:[
      {
        lang: 'english',
        num: 8
      },
      {
        lang:'spanish',
        num:4
      }
    ]
  },
  {
    key: 'she',
    val: 7,
    translations:[
      {
        lang: 'english',
        num: 10
      },
      {
        lang:'spanish',
        num:4
      }
    ]
  }
]
var termArr2 = [
  {
    key: 'me',
    val: 4,
    translations:[
      {
        lang: 'english',
        num: 89
      },
      {
        lang:'spanish',
        num:10
      }
    ]
  },
  {
    key: 'he',
    val: 3,
    translations:[
      {
        lang: 'english',
        num: 8
      },
      {
        lang:'spanish',
        num:2
      }
    ]
  }
]

var result = merge.merge_nested(termArr,termArr2);
console.log(result,'result');
