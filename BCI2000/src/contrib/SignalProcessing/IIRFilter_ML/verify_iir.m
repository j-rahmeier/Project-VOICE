function c = verify_iir(filename, maxBlocks)

if(nargin < 2)
    maxBlocks = 0;
end

fprintf(1,'loading file ...');
[signal, ~, ~] = load_bcidat(filename, '-calibrated');
done;

if(maxBlocks > 0)
    maxSamples = floor(maxBlocks * SampleBlockSize);
    if(maxSamples < size(signal,1))
      signal = signal(1:maxSamples, :);
    end
end

samples = size(signal,1);
channels = size(signal,2);

a = [1 0 -1];
paramA = [sprintf('blubb list DenominatorA= %d', length(a)) sprintf(' %0.10g', a)];
b = [1 2 1];
paramB = [sprintf('blubb list NumeratorB= %d', length(b)) sprintf(' %0.10g', b)];
zi = [1 -1] * 1000;
zi = repmat(zi', [1 8]);
paramZi = [sprintf('blubb matrix DelaysZi= %d %d', size(zi,1), size(zi,2)) sprintf(' %0.10g', zi')];

fprintf(1,'Running filter()...\n');
y = filter(b, a, double(signal), zi);

bci2000path -AddToSystemPath tools/cmdline
fprintf(1,'Running BCI2000 filter chain ...\n');
chain_result = bci2000chain(filename, 'IIRFilter_ML', paramB, paramA, paramZi);
done;
c = [chain_result.Signal y];
e = sqrt(sum(sum((chain_result.Signal - y).^2 )/samples)/channels);
cor = corr(c);
cor = cor(1:channels, channels+1:2*channels);
cor = diag(cor);
cor = sqrt(sum(cor.^2)/length(cor));

figure;
plot(c);
title(['IIRFilter comparison (RMS error: ' num2str(e) ', corr: ' num2str(cor) ')']);
legend('IIRFilter_ML','filter()');


function done
fprintf(1,' done.\n');
