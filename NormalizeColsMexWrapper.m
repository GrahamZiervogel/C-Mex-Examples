function B = NormalizeColsMexWrapper(A,p)
% M-function wrapper for parsing the inputs
if nargin < 2
    if nargin < 1
        error('Not enough input arguments');
    end
    p = 2; % p is unspecified, set default value
end

if ~isreal(A) || ndims(A) ~= 2 || issparse(A) || ~isa(A,'double') %#ok<ISMAT>
    error('A must be a real 2D full double array.');
elseif ~isreal(p) || ~isa(p,'double') || numel(p) ~= 1
    error('p must be a real double scalar.');
end

B=NormalizeColsMex(A,p);

end

