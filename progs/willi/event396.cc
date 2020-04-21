

//#include "../../stalgo/src/coredefs.h" 

#include "CORE/CORE.h"


using namespace std;

double Neg(const double & d) {
	return -(d);
}

int main() {

	// (depth: 1, ElementCount: 1, refcount 11)
	double seq45 =  double(1);

	// (depth: 1, ElementCount: 1, refcount 8)
	double seq46 =  double(2);

	// (depth: 1, ElementCount: 1, refcount 13)
	double seq65 =  double(0);

	// (depth: 1, ElementCount: 1, refcount 9)
	double seq66 =  double(1);

	// (depth: 1, ElementCount: 1, refcount 2)
	double seq72 =  double(3);

	// (depth: 1, ElementCount: 1, refcount 3)
	double seq73 =  double(1e+06);

	// (depth: 1, ElementCount: 1, refcount 11)
	double seq138 =  double(2);

	// (depth: 1, ElementCount: 1, refcount 4)
	double seq165 =  double("272");

	// (depth: 1, ElementCount: 1, refcount 4)
	double seq166 =  double("208");

	// (depth: 1, ElementCount: 1, refcount 26)
	double seq169 =  double("272");

	// (depth: 1, ElementCount: 1, refcount 26)
	double seq170 =  double("304");

	// (depth: 1, ElementCount: 1, refcount 3)
	double seq173 =  double("352");

	// (depth: 1, ElementCount: 1, refcount 5)
	double seq174 =  double("368");

	// (depth: 1, ElementCount: 1, refcount 5)
	double seq189 =  double("96");

	// (depth: 1, ElementCount: 1, refcount 14)
	double seq221 =  double("496");

	// (depth: 1, ElementCount: 1, refcount 5)
	double seq250 =  double("32");

	// (depth: 2, ElementCount: 3, refcount 1)
	double seq384 = seq189 + seq221;

	// (depth: 3, ElementCount: 5, refcount 2)
	double seq385 = seq384 / seq138;

	// (depth: 2, ElementCount: 3, refcount 1)
	double seq386 = seq250 + seq174;

	// (depth: 3, ElementCount: 5, refcount 2)
	double seq387 = seq386 / seq138;

	// (depth: 2, ElementCount: 3, refcount 1)
	double seq388 = seq221 - seq189;

	// (depth: 3, ElementCount: 5, refcount 2)
	double seq389 = seq388 * seq46;

	// (depth: 2, ElementCount: 3, refcount 1)
	double seq390 = seq174 - seq250;

	// (depth: 3, ElementCount: 5, refcount 2)
	double seq391 = seq390 * seq46;

	// (depth: 4, ElementCount: 7, refcount 1)
	double seq392 = seq391 / seq138;

	// (depth: 5, ElementCount: 13, refcount 7)
	double seq393 = seq387 + seq392;

	// (depth: 4, ElementCount: 7, refcount 1)
	double seq394 = seq391 / seq138;

	// (depth: 5, ElementCount: 13, refcount 7)
	double seq395 = seq387 - seq394;

	// (depth: 4, ElementCount: 7, refcount 1)
	double seq396 = seq389 / seq138;

	// (depth: 5, ElementCount: 13, refcount 7)
	double seq397 = seq385 + seq396;

	// (depth: 4, ElementCount: 7, refcount 1)
	double seq398 = seq389 / seq138;

	// (depth: 5, ElementCount: 13, refcount 7)
	double seq399 = seq385 - seq398;

	// (depth: 2, ElementCount: 3, refcount 3)
	double seq663 = seq174 - seq170;

	// (depth: 2, ElementCount: 3, refcount 3)
	double seq664 = seq173 - seq169;

	// (depth: 3, ElementCount: 7, refcount 1)
	double seq665 = seq663 * seq663;

	// (depth: 3, ElementCount: 7, refcount 1)
	double seq666 = seq664 * seq664;

	// (depth: 4, ElementCount: 15, refcount 1)
	double seq667 = seq666 + seq665;

	// (depth: 5, ElementCount: 16, refcount 2)
	double seq668 = sqrt(seq667);

	// (depth: 6, ElementCount: 20, refcount 1)
	double seq669 = seq664 / seq668;

	// (depth: 6, ElementCount: 20, refcount 1)
	double seq670 = seq663 / seq668;

	// (depth: 2, ElementCount: 3, refcount 3)
	double seq671 = seq170 - seq166;

	// (depth: 2, ElementCount: 3, refcount 3)
	double seq672 = seq169 - seq165;

	// (depth: 3, ElementCount: 7, refcount 1)
	double seq673 = seq671 * seq671;

	// (depth: 3, ElementCount: 7, refcount 1)
	double seq674 = seq672 * seq672;

	// (depth: 4, ElementCount: 15, refcount 1)
	double seq675 = seq674 + seq673;

	// (depth: 5, ElementCount: 16, refcount 2)
	double seq676 = sqrt(seq675);

	// (depth: 6, ElementCount: 20, refcount 2)
	double seq677 = seq672 / seq676;

	// (depth: 6, ElementCount: 20, refcount 2)
	double seq678 = seq671 / seq676;

	// (depth: 7, ElementCount: 21, refcount 2)
	double seq689 = Neg(seq670);

	// (depth: 7, ElementCount: 21, refcount 2)
	double seq690 = Neg(seq669);

	// (depth: 8, ElementCount: 42, refcount 3)
	double seq701 = seq689 + seq678;

	// (depth: 8, ElementCount: 42, refcount 3)
	double seq702 = seq690 + seq677;

	// (depth: 9, ElementCount: 85, refcount 1)
	double seq703 = seq701 * seq701;

	// (depth: 9, ElementCount: 85, refcount 1)
	double seq704 = seq702 * seq702;

	// (depth: 10, ElementCount: 171, refcount 1)
	double seq705 = seq704 + seq703;

	// (depth: 11, ElementCount: 172, refcount 2)
	double seq706 = sqrt(seq705);

	// (depth: 12, ElementCount: 215, refcount 3)
	double seq707 = seq702 / seq706;

	// (depth: 12, ElementCount: 215, refcount 3)
	double seq708 = seq701 / seq706;

	// (depth: 13, ElementCount: 237, refcount 1)
	double seq713 = seq689 * seq707;

	// (depth: 13, ElementCount: 237, refcount 1)
	double seq714 = seq690 * seq708;

	// (depth: 14, ElementCount: 475, refcount 1)
	double seq715 = seq714 - seq713;

	// (depth: 15, ElementCount: 476, refcount 1)
	double seq716 = fabs(seq715);

	// (depth: 13, ElementCount: 236, refcount 1)
	double seq717 = seq678 * seq707;

	// (depth: 13, ElementCount: 236, refcount 1)
	double seq718 = seq677 * seq708;

	// (depth: 14, ElementCount: 473, refcount 1)
	double seq719 = seq718 - seq717;

	// (depth: 15, ElementCount: 474, refcount 1)
	double seq720 = fabs(seq719);

	// (depth: 16, ElementCount: 476, refcount 1)
	double seq721 = seq45 / seq720;

	// (depth: 16, ElementCount: 478, refcount 1)
	double seq722 = seq45 / seq716;

	// (depth: 17, ElementCount: 955, refcount 1)
	double seq723 = seq722 + seq721;

	// (depth: 18, ElementCount: 957, refcount 2)
	double seq724 = seq723 / seq46;

	// (depth: 19, ElementCount: 1173, refcount 25)
	double seq725 = seq724 * seq708;

	cout << "seq725: " << endl;
	cout << seq725 << endl;

	// (depth: 19, ElementCount: 1173, refcount 25)
	double seq726 = seq724 * seq707;

	// (depth: 1, ElementCount: 1, refcount 1)
	double seq3865 =  double(1);

	// (depth: 1, ElementCount: 1, refcount 1)
	double seq3866 =  double(5);

	// (depth: 2, ElementCount: 3, refcount 2)
	double seq3867 = seq3865 / seq3866;

	// (depth: 3, ElementCount: 5, refcount 1)
	double seq3868 = seq66 - seq3867;

	// (depth: 6, ElementCount: 19, refcount 1)
	double seq3869 = seq3868 * seq395;

	// (depth: 6, ElementCount: 17, refcount 1)
	double seq3870 = seq3867 * seq393;

	// (depth: 7, ElementCount: 37, refcount 1)
	double seq3871 = seq3870 + seq3869;

	// (depth: 1, ElementCount: 1, refcount 1)
	double seq3872 =  double(0);

	// (depth: 1, ElementCount: 1, refcount 1)
	double seq3873 =  double(5);

	// (depth: 2, ElementCount: 3, refcount 2)
	double seq3874 = seq3872 / seq3873;

	// (depth: 3, ElementCount: 5, refcount 1)
	double seq3875 = seq66 - seq3874;

	// (depth: 6, ElementCount: 19, refcount 1)
	double seq3876 = seq3875 * seq395;

	// (depth: 6, ElementCount: 17, refcount 1)
	double seq3877 = seq3874 * seq393;

	// (depth: 7, ElementCount: 37, refcount 1)
	double seq3878 = seq3877 + seq3876;

	// (depth: 1, ElementCount: 1, refcount 1)
	double seq3879 =  double(1);

	// (depth: 1, ElementCount: 1, refcount 1)
	double seq3880 =  double(4);

	// (depth: 2, ElementCount: 3, refcount 2)
	double seq3881 = seq3879 / seq3880;

	// (depth: 3, ElementCount: 5, refcount 1)
	double seq3882 = seq66 - seq3881;

	// (depth: 6, ElementCount: 19, refcount 1)
	double seq3883 = seq3882 * seq399;

	// (depth: 6, ElementCount: 17, refcount 1)
	double seq3884 = seq3881 * seq397;

	// (depth: 7, ElementCount: 37, refcount 1)
	double seq3885 = seq3884 + seq3883;

	// (depth: 1, ElementCount: 1, refcount 1)
	double seq3886 =  double(0);

	// (depth: 1, ElementCount: 1, refcount 1)
	double seq3887 =  double(4);

	// (depth: 2, ElementCount: 3, refcount 2)
	double seq3888 = seq3886 / seq3887;

	// (depth: 3, ElementCount: 5, refcount 1)
	double seq3889 = seq66 - seq3888;

	// (depth: 6, ElementCount: 19, refcount 1)
	double seq3890 = seq3889 * seq399;

	// (depth: 6, ElementCount: 17, refcount 1)
	double seq3891 = seq3888 * seq397;

	// (depth: 7, ElementCount: 37, refcount 1)
	double seq3892 = seq3891 + seq3890;

	// (depth: 8, ElementCount: 75, refcount 2)
	double seq3893 = seq3871 - seq3878;

	// (depth: 8, ElementCount: 75, refcount 2)
	double seq3894 = seq3892 - seq3885;

	// (depth: 9, ElementCount: 151, refcount 1)
	double seq3895 = seq3893 * seq3893;

	// (depth: 9, ElementCount: 151, refcount 1)
	double seq3896 = seq3894 * seq3894;

	// (depth: 10, ElementCount: 303, refcount 1)
	double seq3897 = seq3896 + seq3895;

	// (depth: 11, ElementCount: 304, refcount 1)
	double seq3898 = sqrt(seq3897);

	// (depth: 20, ElementCount: 2347, refcount 1)
	double seq3899 = seq725 * seq725;

	// (depth: 20, ElementCount: 2347, refcount 1)
	double seq3900 = seq726 * seq726;

	// (depth: 21, ElementCount: 4695, refcount 1)
	double seq3901 = seq3900 + seq3899;

	// (depth: 22, ElementCount: 4696, refcount 2)
	double seq3902 = sqrt(seq3901);

	// (depth: 23, ElementCount: 5870, refcount 1)
	double seq3903 = seq726 / seq3902;

	// (depth: 23, ElementCount: 5870, refcount 1)
	double seq3904 = seq725 / seq3902;

	// (depth: 12, ElementCount: 306, refcount 2)
	double seq3905 = seq72 * seq3898;

	// (depth: 24, ElementCount: 6177, refcount 1)
	double seq3906 = seq3905 * seq3904;

	// (depth: 24, ElementCount: 6177, refcount 1)
	double seq3907 = seq3905 * seq3903;

	// (depth: 25, ElementCount: 6179, refcount 1)
	double seq3919 = seq170 + seq3906;

	// (depth: 25, ElementCount: 6179, refcount 1)
	double seq3920 = seq169 + seq3907;

	// (depth: 20, ElementCount: 2347, refcount 1)
	double seq3921 = seq725 * seq725;

	// (depth: 20, ElementCount: 2347, refcount 1)
	double seq3922 = seq726 * seq726;

	// (depth: 21, ElementCount: 4695, refcount 1)
	double seq3923 = seq3922 + seq3921;

	// (depth: 22, ElementCount: 4696, refcount 2)
	double seq3924 = sqrt(seq3923);

	// (depth: 23, ElementCount: 5870, refcount 1)
	double seq3925 = seq726 / seq3924;

	// (depth: 23, ElementCount: 5870, refcount 1)
	double seq3926 = seq725 / seq3924;

	// (depth: 24, ElementCount: 5872, refcount 1)
	double seq3927 = seq73 * seq3926;

	// (depth: 24, ElementCount: 5872, refcount 1)
	double seq3928 = seq73 * seq3925;

	// (depth: 26, ElementCount: 12052, refcount 6)
	double seq3929 = seq3920 + seq3928;

	// (depth: 26, ElementCount: 12052, refcount 6)
	double seq3930 = seq3919 + seq3927;

	// (depth: 27, ElementCount: 12054, refcount 2)
	double seq3931 = seq3929 - seq169;

	// (depth: 1, ElementCount: 1, refcount 1)
	double seq3960 =  double(1);

	// (depth: 1, ElementCount: 1, refcount 1)
	double seq3961 =  double(4);

	// (depth: 2, ElementCount: 3, refcount 2)
	double seq3962 = seq3960 / seq3961;

	// (depth: 3, ElementCount: 5, refcount 1)
	double seq3963 = seq66 - seq3962;

	// (depth: 6, ElementCount: 19, refcount 1)
	double seq3964 = seq3963 * seq399;

	// (depth: 6, ElementCount: 17, refcount 1)
	double seq3965 = seq3962 * seq397;

	// (depth: 7, ElementCount: 37, refcount 3)
	double seq3966 = seq3965 + seq3964;

	// (depth: 27, ElementCount: 12090, refcount 1)
	double seq3967 = seq3929 - seq3966;

	// (depth: 28, ElementCount: 12092, refcount 1)
	double seq3968 = seq3967 * seq170;

	// (depth: 8, ElementCount: 39, refcount 1)
	double seq3969 = seq3966 - seq169;

	// (depth: 27, ElementCount: 12092, refcount 1)
	double seq3970 = seq3969 * seq3930;

	// (depth: 29, ElementCount: 24185, refcount 1)
	double seq3971 = seq3970 + seq3968;

	// (depth: 30, ElementCount: 36240, refcount 1)
	double seq3972 = seq3971 / seq3931;

	// (depth: 20, ElementCount: 2347, refcount 1)
	double seq4036 = seq725 * seq725;

	// (depth: 20, ElementCount: 2347, refcount 1)
	double seq4037 = seq726 * seq726;

	// (depth: 21, ElementCount: 4695, refcount 1)
	double seq4038 = seq4037 + seq4036;

	// (depth: 31, ElementCount: 36242, refcount 1)
	double seq4039 = seq3972 - seq170;

	// (depth: 8, ElementCount: 39, refcount 1)
	double seq4040 = seq3966 - seq169;

	// (depth: 32, ElementCount: 37416, refcount 1)
	double seq4041 = seq4039 * seq725;

	// (depth: 20, ElementCount: 1213, refcount 1)
	double seq4042 = seq4040 * seq726;

	// (depth: 33, ElementCount: 38630, refcount 1)
	double seq4043 = seq4042 + seq4041;

	// (depth: 34, ElementCount: 43326, refcount 1)
	double seq4044 = seq4043 / seq4038;

	// (depth: 35, ElementCount: 43328, refcount 2)
	double seq4045 = seq4044 + seq65;

	// (depth: 36, ElementCount: 43330, refcount 2)
	double seq4069 = seq4045 - seq65;

	// (depth: 37, ElementCount: 44504, refcount 1)
	double seq4070 = seq726 * seq4069;

	// (depth: 37, ElementCount: 44504, refcount 1)
	double seq4071 = seq725 * seq4069;

	// (depth: 38, ElementCount: 44506, refcount 2)
	double seq4072 = seq170 + seq4071;

	// (depth: 38, ElementCount: 44506, refcount 2)
	double seq4073 = seq169 + seq4070;

	// (depth: 2, ElementCount: 3, refcount 2)
	double seq4234 = seq166 - seq170;

	// (depth: 2, ElementCount: 3, refcount 2)
	double seq4235 = seq165 - seq169;

	// (depth: 2, ElementCount: 3, refcount 2)
	double seq4236 = seq170 - seq170;

	// (depth: 2, ElementCount: 3, refcount 2)
	double seq4237 = seq169 - seq169;

	// (depth: 39, ElementCount: 44508, refcount 2)
	double seq4238 = seq4072 - seq170;

	// (depth: 39, ElementCount: 44508, refcount 2)
	double seq4239 = seq4073 - seq169;

	// (depth: 3, ElementCount: 7, refcount 1)
	double seq4240 = seq4236 * seq4235;

	// (depth: 3, ElementCount: 7, refcount 1)
	double seq4241 = seq4237 * seq4234;

	// (depth: 4, ElementCount: 15, refcount 1)
	double seq4242 = seq4241 - seq4240;

	// (depth: 3, ElementCount: 7, refcount 1)
	double seq4243 = seq4236 - seq4234;

	// (depth: 3, ElementCount: 7, refcount 1)
	double seq4244 = seq4237 - seq4235;

	// (depth: 4, ElementCount: 8, refcount 1)
	double seq4245 = Neg(seq4243);

	// (depth: 40, ElementCount: 44516, refcount 1)
	double seq4246 = seq4238 * seq4244;

	// (depth: 40, ElementCount: 44517, refcount 1)
	double seq4247 = seq4239 * seq4245;

	// (depth: 41, ElementCount: 89034, refcount 1)
	double seq4248 = seq4247 + seq4246;

	// (depth: 42, ElementCount: 89050, refcount 2)
	double seq4249 = seq4242 / seq4248;

	cout << "seq4249: " << endl;
	cout << seq4249 << endl;

	// (depth: 43, ElementCount: 133559, refcount 1)
	double seq4250 = seq4249 * seq4238;

	cout << "seq4250: " << endl;
	if(seq4250 == 0)
		cout << "seq4250 is zero" << endl;
	cout << seq4250 << endl;

	// (depth: 43, ElementCount: 133559, refcount 1)
	double seq4251 = seq4249 * seq4239;

	cout << "seq4251: " << endl;
	cout << seq4251 << endl;

	// (depth: 44, ElementCount: 133561, refcount 2)
	double seq4252 = seq170 + seq4250;

	cout << "seq4252: " << endl;
	cout << seq4252 << endl;

	// (depth: 44, ElementCount: 133561, refcount 2)
	double seq4253 = seq169 + seq4251;

	cout << "seq4253: " << endl;
	cout << seq4253 << endl;

	// (depth: 20, ElementCount: 2347, refcount 1)
	double seq4254 = seq725 * seq725;

	// (depth: 20, ElementCount: 2347, refcount 1)
	double seq4255 = seq726 * seq726;

	// (depth: 21, ElementCount: 4695, refcount 1)
	double seq4256 = seq4255 + seq4254;

	// (depth: 45, ElementCount: 133563, refcount 1)
	double seq4257 = seq4252 - seq170;

	cout << "seq4257: " << endl;
	cout << seq4257 << endl;

	// (depth: 45, ElementCount: 133563, refcount 1)
	double seq4258 = seq4253 - seq169;

	cout << "seq4258: " << endl;
	cout << seq4258 << endl;

	// (depth: 46, ElementCount: 134737, refcount 1)
	double seq4259 = seq4257 * seq725;

	cout << "seq4259: " << endl;
	cout << seq4259 << endl;

	// (depth: 46, ElementCount: 134737, refcount 1)
	double seq4260 = seq4258 * seq726;

	cout << "seq4260: " << endl;
	cout << seq4260 << endl;

	// (depth: 47, ElementCount: 269475, refcount 1)
	double seq4261 = seq4260 + seq4259;

	cout << "seq4261: " << endl;
	cout << seq4261 << endl;

	// (depth: 48, ElementCount: 274171, refcount 1)
	double seq4262 = seq4261 / seq4256;

	cout << "seq4262: " << endl;
	cout << seq4262 << endl;

	// (depth: 49, ElementCount: 274173, refcount 1)
	double seq4263 = seq4262 + seq65;

	cout << "seq4263: " << seq4263 << endl;
}
